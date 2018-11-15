#ifndef RUN_AROUND_PLATFORM_H
#define RUN_AROUND_PLATFORM_H

#define assert(expression) if (!(expression)) { *(int *)0 = 0; }

// TODO(ebuchholz): figure out how to organize includes a little better
#include "run_around_math.h"

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

// Makes it easier to wrap up in WebIDL
// NOTE(ebuchholz): emscripten+idl can't handle pointers to primitives, so these have to be void
// pointers
struct float_mesh_attribute {
    void *values;
    int count;
};

struct int_mesh_attribute {
    void *values;
    int count;
};

struct loaded_mesh_asset {
    int key;
    float_mesh_attribute positions;
    float_mesh_attribute texCoords;
    float_mesh_attribute normals;
    int_mesh_attribute indices;
};

enum render_command_type {
    RENDER_COMMAND_MESH,
    RENDER_COMMAND_SET_CAMERA
};

struct render_command_header{
    render_command_type type;
};

struct render_mesh_command {
    int key;
};

struct render_command_set_camera {
    matrix4x4 viewMatrix;
    matrix4x4 projMatrix;
};

struct render_command_list {
    memory_arena memory;
};

struct game_input {
    bool forwardButton;
    bool backButton;
    bool leftButton;
    bool rightButton;
    bool upButton;
    bool downButton;

    bool turnUpButton;
    bool turnDownButton;
    bool turnLeftButton;
    bool turnRightButton;

    bool pointerDown;
    bool pointerJustDown;
    int pointerX;
    int pointerY;
};

struct game_memory {
    void *memory;
    unsigned int memoryCapacity;
};


#endif
