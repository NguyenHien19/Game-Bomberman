#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_utils.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Game Dodger";

SDL_Texture* loadTexture( std::string path );
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* gTexture = NULL;

int main(int argc, char* argv[])
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    loadMedia(gTexture,renderer);

    SDL_Event e;
    while (1) {
        SDL_RenderClear( renderer );
        SDL_RenderCopy( renderer, gTexture, NULL, NULL );
        SDL_RenderPresent( renderer );

        if (SDL_PollEvent(&e) && e.type == SDL_QUIT)
            break;
    }

    waitUntilKeyPressed();
    quitSDL(window, renderer, gTexture);
    return 0;
}

