#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "mathlib.h"
#include "camera.h"
#include "objects.h"
#include "rendering.h"
//#include "values.h"

int const WINDOW_WIDTH = 800;
int const WINDOW_LENGHT = 600;
int const MAX_MARCHING_ITERATION = 100;
float const MAX_VIEW_DISTANCE = 50;
float const COLIDE_PRECISION = (float)0.01;
float const EPSILON = 0.01f;
int INTERN_TICKS = 0;

float const FOV = (float)1.222; // 70°

Sphere* spheres;

void SDL_ExitWithError(const char* message);


int main(int argc, char *argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_ExitWithError("initialise");
	}
	else
	{
		SDL_version nb;
		SDL_VERSION(&nb);
		printf("SDL correctly initalised ! Version : %d.%d.%d\n", nb.major, nb.minor, nb.patch);
	}
	 
	window = SDL_CreateWindow("M3d v0.0.4", 
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   WINDOW_WIDTH, WINDOW_LENGHT,
							   0);

	if (window == NULL)
		SDL_ExitWithError("create a window");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
		SDL_ExitWithError("create the renderer");
	/*--------------------------------------*/
	/*
	if (SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE) != 0)
		SDL_Log("SDL could not change render color ! SDL error : %s\n", SDL_GetError());

	if (SDL_RenderDrawPoint(renderer, 400, 300) != 0)
		SDL_Log("SDL could not draw a point ! SDL error : %s\n", SDL_GetError());
	
	if (SDL_RenderDrawLine(renderer, 100, 200, 200, 250) != 0)
		SDL_Log("SDL could not draw a line ! SDL error : %s\n", SDL_GetError());
	*/
	//Test ----------------------------------
	Camera* c;
	c = malloc(sizeof(Camera));
	*c = (Camera){  0, 0, 1,
					0, 0, 0,
					1 };
	Vect3d* Rays = GeneratePointsEcran(c);
	RotatePointsEcran(Rays, 0.2f, 0.f, 0.f);
	int k = WINDOW_LENGHT * WINDOW_WIDTH - 1;
	printf("Test vecteur : %.4f, %.4f, %.4f \n", Rays[0].x, Rays[0].y, Rays[0].z);
	printf("Test vecteur : %.4f, %.4f, %.4f \n", Rays[k].x, Rays[k].y, Rays[k].z);
	/*RotatePointsEcran(Rays, (float)1.5708, 0, 0);
	printf("Après rotation de pi/2 :\n");
	printf("Test vecteur : %.4f, %.4f, %.4f \n", Rays[0].x, Rays[0].y, Rays[0].z);
	printf("Test vecteur : %.4f, %.4f, %.4f \n", Rays[k].x, Rays[k].y, Rays[k].z);
	*/
	int i;
	
	spheres = malloc(sizeof(Sphere));
	spheres[0] = (Sphere){ 0, 10, 1, 1 };
	Color rayColor;
	Vect3d camPos = (Vect3d){ c->posx, c->posy, c->posz };

	for (i = 0; i < 500; i++)
	{
		for (k = 0; k < WINDOW_LENGHT * WINDOW_WIDTH; k++)
		{
			raymarcheRay(&rayColor, camPos, Rays[k]);
			//printf("x: %i , y: %i , R: %f , G: %g , B: %f \n", k % WINDOW_WIDTH, WINDOW_LENGHT - k / WINDOW_WIDTH, rayColor.R, rayColor.G, rayColor.B);
			SDL_SetRenderDrawColor(renderer, rayColor.R, rayColor.G, rayColor.B, rayColor.A);
			SDL_RenderDrawPoint(renderer, k % WINDOW_WIDTH, WINDOW_LENGHT - k / WINDOW_WIDTH);

			
		}
		SDL_RenderPresent(renderer);
		//Rotation de la caméra
		RotatePointsEcran(Rays, -0.001f, 0.f, 0.f);
		//SDL_Delay(50);
		INTERN_TICKS++;
	}
	
	//Fin Test ------------------------------

	free(c);
	free(Rays);

	//SDL_RenderPresent(renderer);
	printf("Done\n");
	/*--------------------------------------*/
	SDL_Delay(3000);
	/*--------------------------------------*/

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char* message)
{
	printf("SDL could not %s ! SDL error : %s\n", message, SDL_GetError());
	exit(EXIT_FAILURE);
}