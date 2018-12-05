#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct FileEntry {
    std::string filename;
    bool is_directory;
} FileEntry;

void EventLoop(SDL_Renderer *renderer);
void Render(SDL_Renderer *renderer, std::vector<FileEntry> currDirContents);
void getDirectoryContents(std::string path, std::vector<FileEntry> *contents);

SDL_Texture* RasterizeText(std::string text, std::string font_file, int font_size, SDL_Color color, SDL_Renderer *renderer);

int main(int argc, char **arv) {
    
    std::string homePath = getenv("HOME");
    std::string initialPath = homePath;
    std::vector<FileEntry> currentDirectoryContents;
    
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error, couldn't initialize SDL2\n");
		exit(1);
	}
	
	SDL_Window *window;
	SDL_Renderer *renderer;
    TTF_Init();

	if(SDL_CreateWindowAndRenderer(800,600, 0, &window, &renderer) != 0) {
		fprintf(stderr, "Error, couldn't create window\n");
		exit(1);
	}

    getDirectoryContents(initialPath, &currentDirectoryContents);
    
    for(int i = 0; i < currentDirectoryContents.size(); i++) {
        std::cout << currentDirectoryContents[i].filename << std::endl;
    }
    
    Render(renderer, currentDirectoryContents);
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


void Render(SDL_Renderer *renderer, std::vector<FileEntry> currDirContents) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    for(int i = 0; i < currDirContents.size(); i++) {
        if(currDirContents[i].is_directory) {
            SDL_SetRenderDrawColor(renderer, 140, 160, 235, SDL_ALPHA_OPAQUE);
        } else {
            SDL_SetRenderDrawColor(renderer, 140, 140, 140, SDL_ALPHA_OPAQUE);
        }
        SDL_Rect rectangle;
        
        rectangle.x = 20;
        rectangle.y = 20 + i*30;
        rectangle.w = 30;
        rectangle.h = 20;
        SDL_RenderFillRect(renderer, &rectangle);
        
        SDL_Color color;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 255;
        SDL_Texture *name = RasterizeText(currDirContents[i].filename, "/Users/drewwilken/Documents/OperatingSystems/file-explore/fonts/OpenSans/OpenSans-Regular.ttf", 1, color, renderer);
        uint32_t format;
        int access, height, width;
        SDL_QueryTexture(name, &format, &access, &width, &height);
        
        rectangle.x = 60;
        rectangle.y = i * 30 + 22;
        rectangle.w = currDirContents[i].filename.length() * 10;
        rectangle.h = 16;
        SDL_RenderCopy(renderer, name, NULL, &rectangle);
        //SDL_SetRenderDrawColor(renderer, 244, 200, 66, SDL_ALPHA_OPAQUE);
        //SDL_RenderFillRect(renderer, &rectangle);
    }
    
    SDL_RenderPresent(renderer);
}

void getDirectoryContents(std::string path, std::vector<FileEntry> *contents) {
    DIR *dir;
    struct dirent *entry;
    
    if((dir = opendir(path.c_str())) != NULL) {
        
        //Loop through each file in current directory
        while((entry = readdir(dir)) != NULL) {
            
            FileEntry file;
            file.filename = entry->d_name;
            file.is_directory = entry->d_type == DT_DIR;
        
            contents->push_back(file);
        }
    }
}

SDL_Texture* RasterizeText(std::string text, std::string font_file, int font_size, SDL_Color color, SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont(font_file.c_str(), font_size);
    
    if (font == NULL) {
        fprintf(stderr, "Error: couldn't open font\n");
        return NULL;
    }
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface == NULL) {
        fprintf(stderr, "Error: could not render text to surface\n");
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "Error: could not convert surface to texture\n");
        return NULL;
    }
    
    SDL_FreeSurface(surface);
    return texture;
}
