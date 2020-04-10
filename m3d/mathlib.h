#pragma once

typedef struct vect2d {
	float x;
	float y;
} Vect2d;

typedef struct vect3d {
	float x;
	float y;
	float z;
} Vect3d;

typedef struct mat2d {
	float a[2][2];
} Mat2d;

typedef struct mat3d {
	float a[3][3];
} Mat3d;

Mat2d matrice2dRotation(float angle);
Mat3d matrice3dRotationZ(float angle);
Vect3d matriceTvect3d(Mat3d M, Vect3d v);
Mat3d matriceRotationCamera(float p, float t, float g);
Vect3d normalize3d(Vect3d p);
float dot(Vect3d u, Vect3d v);
float norme(Vect3d);
