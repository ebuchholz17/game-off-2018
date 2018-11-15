#ifndef RUN_AROUND_GAME_H
#define RUN_AROUND_GAME_H

#include "run_around_platform.h"
#include "run_around_math.cpp"

#define DELTA_TIME (1.0f / 60.0f)

enum mesh_key {
    MESH_KEY_SPHERE,
    MESH_KEY_TREE
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

    bool gameInitialized;

    vector3 debugCameraPos;
    quaternion debugCameraRotation;
};

#endif
