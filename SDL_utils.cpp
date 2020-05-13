#include "SDL_utils.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL.h>

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) logSDLError(std::cout, "CreateImg", true);

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
		logSDLError(std::cout, "InitAudio", true);

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		logSDLError(std::cout, "OpenAudio", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* gTexture, Mix_Chunk* sound)
{
    SDL_DestroyTexture(gTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_FreeChunk(sound);
	Mix_CloseAudio();

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 &&(e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture(std::string path, SDL_Texture* &gTexture, SDL_Renderer* &renderer)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL) logSDLError(std::cout, "LoadImg", true);
    else
    {
        Uint32 colorkey = SDL_MapRGB(loadedSurface->format, 255, 255, 255);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorkey);

        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == NULL)
            logSDLError(std::cout, "CreateTexture", true);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void loadMedia(SDL_Texture* &gTexture, SDL_Renderer* &renderer,std::string path)
{
    gTexture = loadTexture(path , gTexture, renderer);
    if(gTexture == NULL) logSDLError(std::cout, "LoadMedia", true);
}

bool collision(SDL_Rect a, SDL_Rect b) // Tham khảo https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
        return false;
    return true;
}


