
#include "core/game.h"

int main()
{
    if (!game_init())
        return 1;

    while (game_is_running()) {
        game_update();
        game_render();
        game_delay();
    }

    return 0;
}