#ifndef RUN_AROUND_GAME_H
#define RUN_AROUND_GAME_H

#include "run_around_platform.h"

enum mesh_key {
    MESH_KEY_SPHERE
};

struct mesh_asset {
    int key;
    // maybe save a copy of vertices and things like that
};

#define MAX_NUM_MESHES 100

struct game_assets {
    memory_arena assetMemory;
    mesh_asset *meshes[MAX_NUM_MESHES];
    int numMeshes;
};

struct game_state {
    memory_arena memory;
    bool assetsInitialized;
    game_assets assets;
};

#endif
