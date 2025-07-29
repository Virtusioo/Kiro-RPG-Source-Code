
#include "debug.h"
#include "config/game.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdarg.h>
#include <stdio.h>  
#include <stdlib.h>  

extern SDL_Renderer* renderer;
extern TTF_Font* debug_font;
extern bool game_running;

void debug_error(const char* fmt, ...)
{
    char buffer[4096];
    SDL_Event event;

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(debug_font, buffer, 0, white, 500);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    int w, h;
    SDL_FRect dst = {20.0f, 20.0f, texture->w, texture->h};
    SDL_RenderTexture(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);

    while (game_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                game_running = false;
                break;
            }
        }
    }
}

void debug_error_popup(const char* text)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text, NULL);
}

