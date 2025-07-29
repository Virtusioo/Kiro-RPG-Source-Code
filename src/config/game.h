
#ifndef confgame_h
#define confgame_h

#ifdef _WIN32
    #define GAME_DRIVER "direct3d12"
#else
    #define GAME_DRIVER NULL
#endif

#define TARGET_FPS 60
#define FRAME_DELAY 1000 / TARGET_FPS

#define GAME_WIDTH 960
#define GAME_HEIGHT 720

#endif