#pragma once


float sceneSDF(Vect3d p);
float shortestDistanceToSurface(Vect3d p, Vect3d marchingDirection, float start, float end);
Vect3d phongIllumination(Vect3d k_a, Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye);
Vect3d phongContribForLight(Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye, Vect3d lightPos, Vect3d lightIntensity);
float getLight(Vect3d p);

void raymarcheRay(Color* color, Vect3d ro, Vect3d rd);
