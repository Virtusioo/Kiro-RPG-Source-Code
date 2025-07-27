
#ifndef game_h
#define game_h

#include <stdbool.h>

#ifdef _WIN32
    #define GAME_DRIVER "direct3d12"
#else
    #define GAME_DRIVER NULL
#endif

#define TARGET_FPS 60
#define FRAME_DELAY 1000 / TARGET_FPS

bool game_init(const char* res_folder);
void game_quit();
bool game_should_continue();
void game_update();
void game_render();
void game_delay();

#endif