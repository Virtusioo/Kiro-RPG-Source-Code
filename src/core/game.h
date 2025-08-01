
#ifndef game_h
#define game_h

#include <stdbool.h>

bool game_init(const char* res_folder);
void game_quit();
void game_hard_exit();
void game_handle_exit();
bool game_should_continue();
void game_display();

#endif