#include <iostream>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "SDL_utils.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;
const string WINDOW_TITLE = "Game Dodger";
const int step = 10;
const int NUM = 8;
int hearts = 3;

SDL_Texture* loadTexture(std::string path);
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* player = NULL;
SDL_Texture* object = NULL;
SDL_Texture* live = NULL;
SDL_Texture* background = NULL;
Mix_Chunk* sound = NULL;
SDL_Event e;

struct Baddies {
    SDL_Rect rect;

    void set() {
        int sizemax = 45;
        int sizemin = 25;
        int size = rand()%(sizemax - sizemin + 1) + sizemin;
        rect.h = size;
        rect.w = size;
        rect.x = rand()%(SCREEN_WIDTH - size + 1);
        rect.y = rand()%(SCREEN_HEIGHT) - SCREEN_HEIGHT;
    }

    void move() {
        Uint32 time = SDL_GetTicks();
        int speed = 1 + 0.00009*time;
        rect.y += speed;
        if (rect.y > SCREEN_HEIGHT) {
            set();
        }
    }

    void render() {
        SDL_RenderCopy(renderer, object, NULL, &rect);
    }
};

struct Status {
    SDL_Rect rect;

    void set(int x, int y, int w, int h) {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }

    void render(SDL_Texture* texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
};

int main(int argc, char* argv[])
{
    srand(time(0));
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    loadMedia(background,renderer, "background.jpg");
    loadMedia(player,renderer, "player.png");
    loadMedia(object,renderer, "baddies.png");
    loadMedia(live,renderer, "heart.jpg");

    Status hplayer;
    hplayer.set(0, SCREEN_HEIGHT*5/6, 50, 50);

    Baddies* baddies = new Baddies[NUM];
    for(int i=0; i<NUM; i++) {
        baddies[i].set();
    }

    while (true) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear( renderer );
        SDL_RenderCopy( renderer, background, NULL, NULL );

        Status* status = new Status[3];
        for(int i=1; i<=hearts; i++) {
            status[i].set(i*30,0, 25, 25);
            status[i].render(live);
        }

        hplayer.render(player);

        for(int i=0; i<NUM; i++) {
            baddies[i].render();
            baddies[i].move();
        }

        for(int i=0; i<NUM; i++) {
            if(collision(hplayer.rect, baddies[i].rect)) {
                sound = Mix_LoadWAV("sound.wav");
                if (sound == NULL)
                    return -1;
                Mix_PlayChannel(-1, sound, 0);

                baddies[i].rect.y = -1;
                hearts --;
                if(hearts == 0) {
                    SDL_Delay(1000);
                    return -1;
                }
            }
        }

        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_KEYDOWN) {
        	switch (e.key.keysym.sym) {
        		case SDLK_ESCAPE: break;
        		case SDLK_LEFT:
        		    hplayer.rect.x = (hplayer.rect.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
        			break;
            	case SDLK_RIGHT:
            	    hplayer.rect.x = (hplayer.rect.x + step) % SCREEN_WIDTH;
            		break;
        		default: break;
			}
        }

        SDL_RenderPresent( renderer );
        delete[]status;
        if (SDL_PollEvent(&e) && e.type == SDL_QUIT)
            break;
    }

    waitUntilKeyPressed();
    delete[]baddies;

    quitSDL(window, renderer, player, sound);
    quitSDL(window, renderer, object, sound);
    quitSDL(window, renderer, live, sound);
    return 0;
}

