#ifndef RUN_AROUND_GAME_H
#define RUN_AROUND_GAME_H

#include "run_around_platform.h"

struct mesh_asset {
    int key;
    // maybe save a copy of vertices and things like that
};

struct game_assets {
    memory_arena assetMemory;
    int numMeshes;
    mesh_asset *meshes;
};

struct game_state {
    bool assetsInitialized;
    game_assets assets;
};

#endif
