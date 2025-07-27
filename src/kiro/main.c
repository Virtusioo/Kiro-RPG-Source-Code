
#include "core/game.h"

int main()
{
    if (!game_init())
        return 1;

    while (game_should_continue()) {
        game_update();
        game_render();
        game_delay();
    }

    return 0;
}