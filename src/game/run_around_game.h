#ifndef RUN_AROUND_GAME_H
#define RUN_AROUND_GAME_H

#include "run_around_platform.h"
#include "run_around_math.cpp"

#define DELTA_TIME (1.0f / 60.0f)

enum mesh_key {
    MESH_KEY_SPHERE,
    MESH_KEY_CUBE
};

struct mesh_asset {
    mesh_key key;
    // maybe save a copy of vertices and things like that
};

enum texture_key {
    TEXTURE_KEY_UV_TEST,
    TEXTURE_KEY_GROUND
};

struct texture_asset {
    texture_key key;
    // maybe save pixels, but maybe don't need it after uploaded to GPU
};

#pragma pack(push, 1)
struct bitmap_header {
    unsigned short type;
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int bitmapOffset;
    unsigned int structSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xResolution;
    int yResolution;
    unsigned int numColors;
    unsigned int numImportantColors;
};
#pragma pack(pop)

#define MAX_NUM_MESHES 100
#define MAX_NUM_TEXTURES 100

struct game_assets {
    memory_arena assetMemory;
    mesh_asset *meshes[MAX_NUM_MESHES];
    int numMeshes;
    texture_asset *textures[MAX_NUM_TEXTURES];
    int numTextures;
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
