#include <math.h>
#include <stdlib.h>

#include "mathlib.h"
#include "values.h"
#include "camera.h"

void RotatePointsEcran(Vect3d* ecran, float phi, float theta, float gamma)
{
	Mat3d rot = matriceRotationCamera(phi, theta, gamma);
	for (int i = 0; i < WINDOW_LENGHT * WINDOW_WIDTH; i++)
	{
		ecran[i] = matriceTvect3d(rot, ecran[i]);
	}
}


Vect3d* GeneratePointsEcran(Camera* c)
{
	Vect3d* ecran;
	int i, j;
	float u, v;
	float deltaX, deltaZ;
	float n;
	ecran = malloc(sizeof(Vect3d) * WINDOW_LENGHT * WINDOW_WIDTH);

	deltaX = c->d * tanf(FOV / 2) / WINDOW_WIDTH;
	deltaZ = deltaX;
	//deltaZ = c->d * tanf(FOV / 2) / WINDOW_LENGHT;

	for (i = 0; i < WINDOW_LENGHT; i++)
	{
		u = (i - WINDOW_LENGHT / 2) * deltaX;
		for (j = 0; j < WINDOW_WIDTH; j++)
		{
			v = (j - WINDOW_WIDTH / 2) * deltaZ;
			n = 1 / sqrtf(v * v + c->d * c->d + u * u);
			ecran[i * WINDOW_WIDTH + j] = (Vect3d){v*n, c->d*n, u*n};
		}
	}
	return ecran;
}