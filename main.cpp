#include <iostream>
#include <SDL.h>

int main(int argc, char **arv) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error, couldn't initialize SDL2\n");
		exit(1);
	}
	
	SDL_Windows *window;
	SDL_Renderer *renderer;

	if(SDL_CreateWindowAndRenderer(800,600, 0, &window, &renderer) != 0) {
		fprintf(stderr, "Error, couldn't create window\n");
		exit(1);
	}

	
	return 0;
}	
