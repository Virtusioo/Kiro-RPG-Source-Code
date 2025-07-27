
#include "game.h"

#include <SDL3/SDL.h>

SDL_Renderer* renderer;
SDL_Window* window;

static SDL_Event event;
static bool is_running = true;
static Uint64 frame_start;

void game_init()
{
    window = SDL_CreateWindow("Kiro", 960, 720, 0);
    renderer = SDL_CreateRenderer(window, GAME_DRIVER);
}

static void game_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool game_is_running()
{
    if (!is_running)
        game_quit();
    return is_running;
}

void game_update()
{
    frame_start = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            is_running = false;
        }
    }
}

void game_render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Draw something here...
    SDL_RenderPresent(renderer);
}

void game_delay()
{
    Uint64 frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frame_time);
    }
}