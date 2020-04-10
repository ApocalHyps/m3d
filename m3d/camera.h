#pragma once

typedef struct camera {
	float posx;
	float posy;
	float posz;
	float et;
	float ep;
	float eg;
	float d;
}Camera;

Vect3d* GeneratePointsEcran(Camera *c);
void RotatePointsEcran(Vect3d* ecran, float phi, float theta, float gamma);
