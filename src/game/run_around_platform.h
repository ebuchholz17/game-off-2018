#ifndef RUN_AROUND_PLATFORM_H
#define RUN_AROUND_PLATFORM_H

struct memory_arena {
    char *base;
    unsigned int size;
    unsigned int capacity;
};

enum render_command_type {
    RENDER_COMMAND_RECTANGLE
};

struct render_rectangle_command {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int color;
};

struct render_command_list {
    memory_arena memory;
};

//struct game_state {
//    memory_arena memory;
//};

#endif
