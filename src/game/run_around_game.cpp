#include "run_around_platform.h";

static int boxPosition = 0;

extern "C" void updateGame (int gameWidth, int gameHeight, unsigned int *pixels) { 
    unsigned int value;
    int startX = boxPosition;
    int endX = startX + 100;
    for (int i = 100; i < 200; ++i) {
        value = 0xffff00ff;
        //value = 0;
        for (int j = startX; j < endX; ++j) {
            //pixels[i * gameWidth + j] = value << 8;
            //++value;
            pixels[i * gameWidth + j] = value;
        }
    }
    if (endX < 1000) { boxPosition++; }
}
