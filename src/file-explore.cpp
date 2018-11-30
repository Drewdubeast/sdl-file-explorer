#include <iostream>
#include <SDL2/SDL.h>

void EventLoop(SDL_Renderer *renderer);
void Render(SDL_Renderer *renderer);

int main(int argc, char **arv) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error, couldn't initialize SDL2\n");
		exit(1);
	}
	
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_CreateWindowAndRenderer(800,600, 0, &window, &renderer) != 0) {
		fprintf(stderr, "Error, couldn't create window\n");
		exit(1);
	}

    Render(renderer);	
	EventLoop(renderer);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}	

void EventLoop(SDL_Renderer *renderer) {
	bool running = true;
	SDL_Event event;
	
	while(running) {
		SDL_WaitEvent(&event);
		
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
		}		
	}
}


void Render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 128, 128, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

  // Create a rectangle
  SDL_Rect rectangle;
  rectangle.x = 100;
  rectangle.y = 100;
  rectangle.w = 200;
  rectangle.h = 75;
  SDL_RenderFillRect(renderer, &rectangle);
   
  SDL_RenderPresent(renderer);
}
