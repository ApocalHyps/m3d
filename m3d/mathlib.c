#include <math.h>
#include <stdlib.h>

#include "mathlib.h"

float dot(Vect3d u, Vect3d v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

float norme(Vect3d p)
{
	return sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
}

Vect3d normalize3d(Vect3d p)
{
	Vect3d r;
	float n = 1.f / sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
	r.x = p.x * n;
	r.y = p.y * n;
	r.z = p.z * n;
	return r;
}

Mat2d matrice2dRotation(float angle)
{
	Mat2d matriceReturned;
	float c = cosf(angle);
	float s = sinf(angle);
	matriceReturned.a[0][0] = c;
	matriceReturned.a[0][1] = -s;
	matriceReturned.a[1][0] = s;
	matriceReturned.a[1][1] = c;
	return matriceReturned;
}

Vect2d matriceTvect2d(Mat2d M, Vect2d v)
{
	Vect2d r;
	r.x = v.x * M.a[0][0] + v.y * M.a[0][1];
	r.y = v.x * M.a[1][0] + v.y * M.a[1][1];
	return r;
}

Mat3d matriceRotationCamera(float p, float t, float g)
{
	Mat3d matriceReturned;
	float cp = cosf(p);
	float sp = sinf(p);
	float ct = cosf(t);
	float st = sinf(t);
	float cg = cosf(g);
	float sg = sinf(g);
	matriceReturned.a[0][0] = cg * cp + sg * st * sp;
	matriceReturned.a[0][1] = -cg * sp + sg * st * cp;
	matriceReturned.a[0][2] = ct * sg;
	matriceReturned.a[1][0] = ct * sp;
	matriceReturned.a[1][1] = ct * cp;
	matriceReturned.a[1][2] = -st;
	matriceReturned.a[2][0] = -sg * cp + cg * st * sp;
	matriceReturned.a[2][1] = sg * sp + cg * st * cp;
	matriceReturned.a[2][2] = cg * ct;
	return matriceReturned;
}

Vect3d matriceTvect3d(Mat3d M, Vect3d v)
{
	Vect3d r;
	r.x = v.x * M.a[0][0] + v.y * M.a[0][1] + v.z * M.a[0][2];
	r.y = v.x * M.a[1][0] + v.y * M.a[1][1] + v.z * M.a[1][2];
	r.z = v.x * M.a[2][0] + v.y * M.a[2][1] + v.z * M.a[2][2];
	return r;
}

Mat3d matrice3dRotationZ(float angle)
{
	Mat3d matriceReturned;
	float c = cosf(angle);
	float s = sinf(angle);
	matriceReturned.a[0][0] = c;
	matriceReturned.a[0][1] = -s;
	matriceReturned.a[1][0] = s;
	matriceReturned.a[1][1] = c;
	matriceReturned.a[0][2] = 0;
	matriceReturned.a[1][2] = 0;
	matriceReturned.a[2][0] = 0;
	matriceReturned.a[2][1] = 0;
	matriceReturned.a[2][2] = 1;
	return matriceReturned;
}