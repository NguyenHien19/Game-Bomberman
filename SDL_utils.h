#ifndef SDL_UTILS__H
#define SDL_UTILS__H

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
             int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE);

void loadMedia(SDL_Texture* &gTexture, SDL_Renderer* &renderer,std::string path);

bool collision(SDL_Rect a, SDL_Rect b);

SDL_Texture* loadTexture(std::string path, SDL_Texture* &gTexture, SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* gTexture, Mix_Chunk* sound);

void waitUntilKeyPressed();

#endif // SDL_UTILS__H
