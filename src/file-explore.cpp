#include <iostream>
#include <SDL2/SDL.h>
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
    SDL_SetRenderDrawColor(renderer, 0, 128, 128, SDL_ALPHA_OPAQUE);
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
        
        SDL_SetRenderDrawColor(renderer, 244, 200, 66, SDL_ALPHA_OPAQUE);
        rectangle.x = 60;
        rectangle.y = i * 30 + 22;
        rectangle.w = currDirContents[i].filename.length() * 10;
        rectangle.h = 16;
        SDL_RenderFillRect(renderer, &rectangle);
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
            
            for(int i = 0; i < contents.size(); i++) {
                if(strncasecmp(contents->at(i).filename.c_str(), file.filename.c_str(), 256) < 0) {
                    index++;
                }
            }
            contents->insert(contents->begin() + index, f);
            
            contents->push_back(file);
        }
    }
}
