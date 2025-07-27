
#include "core/game.h"

int main()
{
    game_init();

    while (game_isrunning()) {
        game_update();
        game_render();
        game_delay();
    }
}