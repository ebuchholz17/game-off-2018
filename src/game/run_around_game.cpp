#include "run_around_platform.h"

colored_rectangle *pushRectangle (rectangle_list *rectangleList) {
    colored_rectangle *result = 0;
    if (rectangleList->numRectangles < rectangleList->maxRectangles) {
        result = rectangleList->rectangles + rectangleList->numRectangles;
        rectangleList->numRectangles++;
    }
    return result;
}

extern "C" void updateGame (int gameWidth, int gameHeight, rectangle_list *rectangleList) { 
    static int startX = 0;
    startX++;
    for (int i = 0; i < 10; ++i) {
        colored_rectangle *rectangle = pushRectangle(rectangleList);
        if (rectangle) {
            rectangle->x = startX +i * 100;
            rectangle->y = i * 100;
            rectangle->width =  100;
            rectangle->height = 100;
            rectangle->color = 0xffff00ff;
        }
    }
}
