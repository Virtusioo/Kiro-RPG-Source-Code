
#include "core/game.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc <= 1) {
        puts("usage: kiro.exe <project>");
        return 1;
    }

    if (!game_init(argv[1])) {
        game_quit();
        return 1;
    }

    while (game_should_continue()) 
        game_display();

    return 0;
}