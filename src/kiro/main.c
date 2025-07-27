
#include "core/game.h"

int main()
{
    game_init();

    while (game_is_running()) {
        game_update();
        game_render();
        game_delay();
    }

    return 0;
}