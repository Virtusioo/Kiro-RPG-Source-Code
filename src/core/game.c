
#include "game.h"
#include "debug.h"
#include "debug_font.h"
#include "file.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
TTF_Font* debug_font = NULL;
bool game_running = true;

static SDL_Event event;
static Uint64 frame_start;

bool game_init(const char* res_folder)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log(SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log(SDL_GetError());
        return false;
    }

    file_mount(res_folder);

    window = SDL_CreateWindow("Kiro", 960, 720, SDL_WINDOW_HIDDEN);

    if (window == NULL) {
        debug_error_popup(SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, GAME_DRIVER);

    if (renderer == NULL) {
        debug_error_popup(SDL_GetError());
        return false;
    }

    SDL_IOStream* io = SDL_IOFromConstMem(debug_font_raw, debug_font_raw_size);
    debug_font = TTF_OpenFontIO(io, true, 35);

    if (debug_font == NULL) {
        debug_error_popup(SDL_GetError());
        return false;
    }

    SDL_SetRenderLogicalPresentation(renderer, 960, 720, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_ShowWindow(window);

    return true;
}

void game_quit()
{
    TTF_CloseFont(debug_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool game_should_continue()
{
    if (!game_running)
        game_quit();
    return game_running;
}

void game_update()
{
    frame_start = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            game_running = false;
        }
    }
}

void game_render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    debug_error("Hello, World");
    SDL_RenderPresent(renderer);
}

void game_delay()
{
    Uint64 frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frame_time);
    }
}

void game_display()
{
    game_update();
    game_render();
    game_delay();
}