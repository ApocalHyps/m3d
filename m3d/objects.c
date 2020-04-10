#include <math.h>
#include "mathlib.h"
#include "objects.h"
#include "rendering.h"
#include "values.h"


float sphereSDF(Vect3d p)
{
	Vect3d camToCenter = (Vect3d){ spheres[0].pos.x - p.x, spheres[0].pos.y - p.y, spheres[0].pos.z - p.z }; // Vect (s-p)
	return sqrtf(camToCenter.x * camToCenter.x + camToCenter.y * camToCenter.y + camToCenter.z * camToCenter.z) - spheres[0].radius;
}

float planXY0SDF(Vect3d p)
{
	return p.z>=0?p.z:-p.z;
}

Vect3d reflect(Vect3d incident, Vect3d normal)
{
	Vect3d r;
	float value = dot(incident, normal);
	r.x = incident.x - 2.f * value * normal.x;
	r.y = incident.y - 2.f * value * normal.y;
	r.z = incident.z - 2.f * value * normal.z;
	return r;
}

Vect3d sphereNormal(Vect3d p, Vect3d c)
{
	return normalize3d((Vect3d) { p.x - c.x, p.y - c.y, p.z - c.z });
}

Vect3d planXYNormal()
{
	return (Vect3d) { 0, 0, 1 };
}

Vect3d estimateNormal(Vect3d p)
{
	return normalize3d((Vect3d){
		sceneSDF((Vect3d) { p.x + EPSILON, p.y, p.z }) - sceneSDF((Vect3d) { p.x - EPSILON, p.y, p.z }),
		sceneSDF((Vect3d) { p.x, p.y + EPSILON, p.z }) - sceneSDF((Vect3d) { p.x, p.y - EPSILON, p.z }),
		sceneSDF((Vect3d) { p.x, p.y, p.z + EPSILON }) - sceneSDF((Vect3d) { p.x, p.y, p.z - EPSILON })
	});
}

