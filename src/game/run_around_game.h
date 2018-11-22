#ifndef RUN_AROUND_GAME_H
#define RUN_AROUND_GAME_H

#include "run_around_platform.h"
#include "run_around_math.cpp"

#define DELTA_TIME (1.0f / 60.0f)

enum mesh_key {
    MESH_KEY_PURPLE_MAN,
    MESH_KEY_CUBE,
    MESH_KEY_CYLINDER,
    MESH_KEY_TEST_GROUND,
    MESH_KEY_TEST_LOOP,
    MESH_KEY_SPHERE
};

struct mesh_asset {
    mesh_key key;
    // maybe save a copy of vertices and things like that
};

enum texture_key {
    TEXTURE_KEY_UV_TEST,
    TEXTURE_KEY_PURPLE,
    TEXTURE_KEY_GROUND,
    TEXTURE_KEY_GREY,
    TEXTURE_KEY_BLUE
};

struct texture_asset {
    texture_key key;
    // maybe save pixels, but maybe don't need it after uploaded to GPU
};

// TODO(ebuchholz): unify mesh key and level mesh key, maybe use hash tables instead of normal 
//arrays to make this more convenient
enum level_mesh_key {
    LEVEL_MESH_KEY_TEST_GROUND,
    LEVEL_MESH_KEY_TEST_LOOP,
    LEVEL_MESH_KEY_SPHERE
};

struct level_mesh {
    level_mesh_key key;
    aabb boundingBox;
    triangle *triangles;
    aabb *triangleAABBs;
    int triangleCount;
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
#define MAX_NUM_LEVEL_MESHES 100

struct game_assets {
    memory_arena assetMemory;

    mesh_asset *meshes[MAX_NUM_MESHES];
    int numMeshes;

    texture_asset *textures[MAX_NUM_TEXTURES];
    int numTextures;

    level_mesh *levelMeshes[MAX_NUM_LEVEL_MESHES];
    int numLevelMeshes;
};

struct debug_camera {
    vector3 pos;
    quaternion rotation;
    int lastPointerX;
    int lastPointerY;
};

enum player_surface_mode {
    PLAYER_SURFACE_MODE_FLOOR,
    PLAYER_SURFACE_MODE_WALL,
    PLAYER_SURFACE_MODE_CEILING
};

#define NUM_COLLISION_SENSORS 9
#define MAX_GROUND_SPEED 6.0f

struct player_state {
    vector3 pos;
    vector3 velocity;
    vector3 groundSpeed; // 2D, relative to surface the player is running on
    aabb boundingBox;
    line collisionSensors[NUM_COLLISION_SENSORS];
    player_surface_mode mode;
    vector3 upDirection;
    vector3 slopeDirection;
};

#define MAX_NUM_LEVEL_CHUNKS 100

// TODO(ebuchholz): arbitrary position/rotation/scale for these guys
struct level_chunk {
    mesh_key meshKey;
    level_mesh_key levelMeshKey;
    vector3 position;
};

struct level_chunks {
    level_chunk chunks[100]; // how many should there be?
    int numChunks;
};

struct level_chunk_intersection_result {
    vector3 intersectionPoint;
    vector3 triangleNormal;
    int sensorIndex;
};

struct game_state {
    memory_arena memory;
    memory_arena tempMemory; // cleared every frame
    bool assetsInitialized;
    game_assets assets;
    bool gameInitialized;

    debug_camera debugCamera;

    player_state player;
    level_chunks levelChunks;
};

#endif
