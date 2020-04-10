
#include "values.h"
#include "mathlib.h"
#include "objects.h"
#include "rendering.h"

#include <math.h>
#include <SDL.h>
#include <stdlib.h>

//#include <stdio.h>

void raymarcheRay(Color* color, Vect3d ro, Vect3d rd)
{
    float distance = shortestDistanceToSurface(ro, rd, 0, MAX_VIEW_DISTANCE);
    //printf("x = %i , y = %i , distance = %f\n", k % WINDOW_WIDTH, WINDOW_LENGHT - k / WINDOW_WIDTH, distance);
    if (distance > MAX_VIEW_DISTANCE - EPSILON)
    {
        //Didn't hit aanything
        color->R = 0;
        color->G = 0;
        color->B = 0;
        return;
    }
    ro = (Vect3d){ ro.x + rd.x * distance, ro.y + rd.y * distance, ro.z + rd.z * distance };

    //float dif = getLight(ro);
    //float dif = 1.f;

    //*color = (Color) { 112 * dif, 168 * dif, 237 * dif, SDL_ALPHA_OPAQUE };
    //return;
    //Phong implementation
    
    Vect3d K_a = (Vect3d){ 0.2f, 0.2f, 0.2f };
    Vect3d K_d = (Vect3d){ 0.7f, 0.2f, 0.2f };
    Vect3d K_s = (Vect3d){ 1.0f, 1.0f, 1.0f };
    float shininess = 3.0f;

    Vect3d rayColor = phongIllumination(K_a, K_d, K_s, shininess, ro, rd);
    //printf("colorR = %f\n", rayColor.x * 255);
    *color = (Color){ min((int)(floorf(rayColor.x * 255)),255),min((int)(floorf(rayColor.y * 255)),255) ,min((int)(floorf(rayColor.z * 255)),255), SDL_ALPHA_OPAQUE};
    //*color = (Color){ (int)floorf(rayColor.x) * 4, (int)floorf(rayColor.y) * 4, (int)floorf(rayColor.z) * 4, SDL_ALPHA_OPAQUE };
    
    return;
}

float getLight(Vect3d p) // Useless since Phong
{
    Vect3d lightPos = (Vect3d){ 4.0f*sinf((float)INTERN_TICKS * 0.1f), 5, 4.0f*cosf((float)INTERN_TICKS*0.1f) };
    Vect3d L = normalize3d((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z });
    Vect3d N = sphereNormal(p, spheres[0].pos);

    float dif = dot(L, N);
    dif = dif > 1.f ? 1.f : dif;
    dif = dif < 0.f ? 0.f : dif;
    float d = shortestDistanceToSurface((Vect3d) { p.x + N.x * 0.02f, p.y + N.y * 0.02f, p.y + N.y * 0.02f }, L, 0, MAX_VIEW_DISTANCE);
    if (d<norme((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z })) dif *= .1f;

    return dif;
}

/**
 * Lighting contribution of a single point light source via Phong illumination.
 *
 * The Vect3d returned is the RGB color of the light's contribution.
 *
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 * lightPos: the position of the light
 * lightIntensity: color/intensity of the light
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
Vect3d phongContribForLight(Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye,
    Vect3d lightPos, Vect3d lightIntensity) {
    Vect3d N = estimateNormal(p);
    Vect3d L = normalize3d((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z });
    Vect3d V = normalize3d((Vect3d) { eye.x - p.x, eye.y - p.y, eye.z - p.z });
    Vect3d R = normalize3d(reflect((Vect3d) {-L.x, -L.y, -L.z}, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    float coef;

    if (dotLN < 0.0f) {
        // Light not visible from this point on the surface
        return (Vect3d) { 0.0f, 0.0f, 0.0f };
    }

    if (dotRV < 0.0f) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        //return lightIntensity * (k_d * dotLN);
        return (Vect3d) { lightIntensity.x* k_d.x* dotLN, lightIntensity.y* k_d.y* dotLN, lightIntensity.z* k_d.z* dotLN };
    }
    coef = powf(dotRV, alpha);
    return (Vect3d) {
        lightIntensity.x * (k_d.x * dotLN +  k_s.x * coef),
        lightIntensity.y * (k_d.y * dotLN + k_s.y * coef),
        lightIntensity.z * (k_d.z * dotLN + k_s.z * coef)
    };
    //return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

/**
 * Lighting via Phong illumination.
 *
 * The Vect3d returned is the RGB color of that point after lighting is applied.
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
Vect3d phongIllumination(Vect3d k_a, Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye) {
    const Vect3d ambientLight = (Vect3d) { 0.5f, 0.5f, 0.5f };
    Vect3d color = (Vect3d){ ambientLight.x * k_a.x, ambientLight.y * k_a.y, ambientLight.z * k_a.z };//ambientLight * k_a;

    Vect3d light1Pos = (Vect3d){ -5.0f * sinf(INTERN_TICKS * 0.1f),
        3.0f,
        5.0f * cosf(INTERN_TICKS * 0.1f) };
    Vect3d light1Intensity = (Vect3d){ 0.4f, 0.4f, 0.4f };

    Vect3d contrib;
    contrib = phongContribForLight(k_d, k_s, alpha, p, eye,
        light1Pos,
        light1Intensity);
    color = (Vect3d){ color.x + contrib.x,color.y + contrib.y, color.z + contrib.z };

    /*
    Vect3d light2Pos = Vect3d(2.0 * sin(0.37 * iTime),
        2.0 * cos(0.37 * iTime),
        2.0);
    Vect3d light2Intensity = Vect3d(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
        light2Pos,
        light2Intensity);
    */
    return color;
}



float sceneSDF(Vect3d p)
{
    //return sphereSDF(p);
    /*
    for each elemnt in scene:
        if elment.sdf(p) < min:
            min = elemnt.sdf(p)
            sdl.renderColor = elemnt.color
    */
    return sphereSDF(p) > planXY0SDF(p) ? planXY0SDF(p):sphereSDF(p);
    //return sphereSDF(p);
}

float shortestDistanceToSurface(Vect3d p, Vect3d marchingDirection, float start, float end)
{
	float depth = start;
    for (int i = 0; i < MAX_MARCHING_ITERATION; i++) {
        float dist = sceneSDF(p);
        if (dist < COLIDE_PRECISION) {
            return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
        p = (Vect3d){ p.x + marchingDirection.x * dist, p.y + marchingDirection.y * dist, p.z + marchingDirection.z * dist };
    }
    return end;
}