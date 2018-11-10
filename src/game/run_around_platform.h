#ifndef RUN_AROUND_PLATFORM_H
#define RUN_AROUND_PLATFORM_H

#define assert(expression) if (!(expression)) { *(int *)0 = 0; }

struct memory_arena {
    void *base;
    unsigned int size;
    unsigned int capacity;
};

// asset loading
enum asset_type {
    ASSET_TYPE_OBJ
};

struct asset_to_load {
    char *path;
    asset_type type;
    int key;
};

struct asset_list {
    int numAssetsToLoad;
    int maxAssetsToLoad;
    asset_to_load *assetsToLoad;
};

struct loaded_mesh_asset {
    int key;
    float *positions;
    float *uvs;
    float *normals;
    int *indices;
};

enum render_command_type {
    RENDER_COMMAND_RECTANGLE,
    RENDER_COMMAND_HORIZONTAL_LINE
};

struct render_command_header{
    render_command_type type;
};

struct render_rectangle_command {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int color;
};

struct render_horizontal_line_command {
    unsigned int color;
    char lineNum; // only first 256 lines
};


struct render_command_list {
    memory_arena memory;
};

struct game_memory {
    memory_arena memory;
};


#endif
