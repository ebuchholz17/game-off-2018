#ifndef RUN_AROUND_PLATFORM_H
#define RUN_AROUND_PLATFORM_H

struct colored_rectangle {
    int x;
    int y;
    int width;
    int height;
    unsigned int color;
};

struct rectangle_list {
    int numRectangles;
    int maxRectangles;
    colored_rectangle *rectangles;
};

#endif
