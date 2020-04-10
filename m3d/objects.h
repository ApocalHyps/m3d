#pragma once

typedef struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
}Color;

typedef struct Object {
	//float (*signedDist)(Vect3d, Vect3d, float);
	Color color;
	Vect3d pos;
}Object;

typedef struct Sphere {
	Vect3d pos;
	float radius;
}Sphere;

float sphereSDF(Vect3d p);
float planXY0SDF(Vect3d p);
Vect3d reflect(Vect3d incident, Vect3d normal);
Vect3d planXYNormal();
Vect3d sphereNormal(Vect3d p, Vect3d c);
Vect3d estimateNormal(Vect3d p);