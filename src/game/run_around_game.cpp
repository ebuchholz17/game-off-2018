#include "run_around_game.h"

// NOTE(ebuchholz): taken from ebrt, and probably RTCD
// TODO(ebuchholz): make a line segment and cylidner collision test
inline bool triangleIntersection (vector3 rayPos, vector3 rayDir, 
                                  vector3 p0, vector3 p1, vector3 p2,
                                  float *t, vector3 *normal, vector3 *intersectionPoint) {
    vector3 ab = p1 - p0;
    vector3 ac = p2 - p0;
    vector3 qp = -rayDir; 

    vector3 n = crossProduct(ab, ac); // triangle normal
    float d = dotProduct(qp, n);
    //if (d <= 0.0f) { // backface culling
    //    return false;
    //}
    float ood = 1.0f / d;
    vector3 ap = rayPos - p0;
    float newT = dotProduct(ap, n);
    newT *= ood;
    if (newT < 0.0f) {
        return false;
    }

    vector3 e = crossProduct(qp, ap);
    float v = dotProduct(ac, e);
    v *= ood;
    if( v < 0.0f || v > 1.0f) {
        return false;
    }
    float w = -dotProduct(ab,e);
    w *= ood;
    if (w < 0.0f || v + w > 1.0f) {
        return false;
    } 

    float u = 1.0f - v - w;

    *t = newT;
    *normal = normalize(crossProduct(ab, ac));
    if (d < 0.0f) {
        *normal = -*normal;
    }
    *intersectionPoint = u * p0 + v * p1 + w * p2;
    return true;
}

static void *pushRenderCommand (render_command_list *renderCommands, 
                         render_command_type type, 
                         unsigned int size) 
{
    void *renderCommand = 0;
    int requiredSize = sizeof(render_command_header) + size;
    if (renderCommands->memory.size + requiredSize < renderCommands->memory.capacity) {
        render_command_header *header = (render_command_header *)((char *)renderCommands->memory.base + 
                                                                  renderCommands->memory.size);
        renderCommands->memory.size += requiredSize;
        header->type = type;
        renderCommand = (char *)header + sizeof(render_command_header);
    }
    else {
        // Out of memory error?
    }
    return renderCommand;
}

static void *allocateMemorySize (memory_arena *memory, unsigned int size) {
    assert(memory->size + size < memory->capacity);
    void *result = (char *)memory->base + memory->size;
    memory->size += size;
    return result;
}

static void readLine (char *data, char **start, char **end) {
    *start = data;
    int index = 0;
    while (data[index] != 0 && data[index] != '\n') {
        ++index;
    }
    *end = data + index;
}

static int stringToInt (char *start, char *end) {
    int result = 0;
    bool negative = false;
    if (*start == '-') {
        negative = true;
        ++start;
    }
    while (start <= end) {
        char digit = *start;
        result *= 10;
        result += (int)(digit - '0');
        ++start;
    }
    if (negative) {
        result = -result;
    }
    return result;
}

// TODO(ebuchholz): just use atof or something
// TODO(ebuchholz): make sure the string is a valid float
static float stringToFloat (char *start, char *end) {
    float result = 0.0;
    float integerPart = 0.0;
    float fractionalPart = 0.0;
    bool negative = false;
    if (*start == '-') {
        negative = true;
        ++start;
    }
    while (start <= end) {
        char digit = *start;
        if (*start == '.') {
            break;
        }
        integerPart *= 10.0f;
        float number = (float)((int)(digit - '0'));
        integerPart += number;
        ++start;
    }
    if (negative) {
        integerPart = -integerPart;
    }
    while (end > start) {
        char digit = *end;
        fractionalPart /= 10.0f;
        float number = (float)((int)(digit - '0'));
        fractionalPart += number;
        --end;
    }
    fractionalPart /= 10.0f;
    if (negative) {
        fractionalPart = -fractionalPart;
    }
    return integerPart + fractionalPart;
}

//static float stringToFloat (char *string) {
//    char *end = string;
//    while (*end != 0) {
//        ++end;
//    }
//    --end;
//    return stringToFloat(string, end);
//}

// TODO(ebuchholz): move somewhere else
static bool stringsAreEqual (int length, char *a, char *b) {
    char *bIndex = b;
    for (int i = 0; i < length; ++i, ++bIndex) {
        if (*bIndex == 0 || a[i] != *bIndex) {
            return false;
        }
    }
    return (*bIndex == 0);
}

static void parseOBJ (void *objData, game_assets *assets, int key, memory_arena *workingMemory) {
    int numMeshes = assets->numMeshes;
    assert(numMeshes < MAX_NUM_MESHES);

    mesh_asset *meshAsset = (mesh_asset *)allocateMemorySize(&assets->assetMemory, sizeof(mesh_asset *));
    assets->meshes[numMeshes] = meshAsset;
    assets->numMeshes++;
    meshAsset->key = (mesh_key)key;

    loaded_mesh_asset *loadedMesh = (loaded_mesh_asset *)allocateMemorySize(workingMemory, sizeof(loaded_mesh_asset));
    loadedMesh->key = key;

    char *start, *end;
    start = (char *)objData;

    unsigned int numPositions = 0;
    unsigned int numNormals = 0;
    unsigned int numTexCoords = 0;
    unsigned int numTriangles = 0;

    while (true) {
        readLine(start, &start, &end);

        char *wordStart = start;
        char *wordEnd = wordStart;
        while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
        int length = (int)(wordEnd - wordStart);

        if (stringsAreEqual(length, wordStart, "v")) {
            ++numPositions;
        }
        else if (stringsAreEqual(length, wordStart, "vt")) {
            ++numTexCoords;
        }
        else if (stringsAreEqual(length, wordStart, "vn")) {
            ++numNormals;
        }
        else if (stringsAreEqual(length, wordStart, "f")) {
            ++numTriangles;
        }

        start = end;
        if (*start == 0) {
            break;
        }
        else {
            start++;
        }
    }

    float (*tempPositions)[3] = (float (*)[3])allocateMemorySize(workingMemory, sizeof(float) * 3 * numPositions);
    float (*tempTexCoords)[2] = (float (*)[2])allocateMemorySize(workingMemory, sizeof(float) * 2 * numTexCoords);
    float (*tempNormals)[3] = (float (*)[3])allocateMemorySize(workingMemory, sizeof(float) * 3 * numNormals);

    loadedMesh->positions.count = 3 * 3 * numTriangles;
    loadedMesh->texCoords.count = 2 * 3 * numTriangles;
    loadedMesh->normals.count = 3 * 3 * numTriangles;
    loadedMesh->indices.count =  3 * numTriangles;

    loadedMesh->positions.values = allocateMemorySize(workingMemory, sizeof(float) * loadedMesh->positions.count);
    loadedMesh->texCoords.values = allocateMemorySize(workingMemory, sizeof(float) * loadedMesh->texCoords.count);
    loadedMesh->normals.values = allocateMemorySize(workingMemory, sizeof(float) * loadedMesh->normals.count);
    loadedMesh->indices.values = allocateMemorySize(workingMemory, sizeof(int) * loadedMesh->indices.count);

    start = (char *)objData;
    int tempPositionIndex = 0;
    int tempTexCoordIndex = 0;
    int tempNormalIndex = 0;

    int positionIndex = 0;
    int uvIndex = 0;
    int normalIndex = 0;
    int indexIndex = 0;
    while (true) {
        readLine(start, &start, &end);

        char *wordStart = start;
        char *wordEnd = wordStart;
        while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
        int length = (int)(wordEnd - wordStart);

        if (stringsAreEqual(length, wordStart, "v")) {
            float (*tempPosition)[3] = &tempPositions[tempPositionIndex];
            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempPosition)[0] = stringToFloat(wordStart, wordEnd - 1);

            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempPosition)[1] = stringToFloat(wordStart, wordEnd - 1);

            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempPosition)[2] = stringToFloat(wordStart, wordEnd - 1);

            tempPositionIndex++;
        }
        else if (stringsAreEqual(length, wordStart, "vt")) {
            float (*tempTexCoord)[2] = &tempTexCoords[tempTexCoordIndex];
            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempTexCoord)[0] = stringToFloat(wordStart, wordEnd - 1);

            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempTexCoord)[1] = stringToFloat(wordStart, wordEnd - 1);

            tempTexCoordIndex++;
        }
        else if (stringsAreEqual(length, wordStart, "vn")) {
            float (*tempNormal)[3] = &tempNormals[tempNormalIndex];
            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempNormal)[0] = stringToFloat(wordStart, wordEnd - 1);

            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempNormal)[1] = stringToFloat(wordStart, wordEnd - 1);

            wordEnd++;
            wordStart = wordEnd;
            while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }
            (*tempNormal)[2] = stringToFloat(wordStart, wordEnd - 1);

            tempNormalIndex++;
        }
        else if (stringsAreEqual(length, wordStart, "f")) {
            // TODO(ebuchholz): handle the case where text coords and/or normals are missing
            for (int i = 0; i < 3; ++i) {
                wordEnd++;
                wordStart = wordEnd;
                while (*wordEnd != ' ' && *wordEnd != '\n' && *wordEnd!= 0) { ++wordEnd; }

                char *indexStart = wordStart;
                char *indexEnd = indexStart;
                while (*indexEnd != ' ' && *indexEnd != '\n' && *indexEnd != '/' && *wordEnd!= 0) { ++indexEnd; }
                if (indexStart != indexEnd) {
                    int index = stringToInt(indexStart, indexEnd - 1) - 1;
                    float *positionsPointer = (float *)loadedMesh->positions.values + (positionIndex * 3);
                    positionsPointer[0] = tempPositions[index][0];
                    positionsPointer[1] = tempPositions[index][1];
                    positionsPointer[2] = tempPositions[index][2];
                    ++positionIndex;
                }
                indexEnd++;
                indexStart = indexEnd;

                while (*indexEnd != ' ' && *indexEnd != '\n' && *indexEnd != '/' && *wordEnd!= 0) { ++indexEnd; }
                if (indexStart != indexEnd) {
                    int index = stringToInt(indexStart, indexEnd - 1) - 1;
                    float *texCoordsPointer = (float *)loadedMesh->texCoords.values + (uvIndex * 2);
                    texCoordsPointer[0] = tempTexCoords[index][0];
                    texCoordsPointer[1] = tempTexCoords[index][1];
                    uvIndex++;
                }
                indexEnd++;
                indexStart = indexEnd;

                while (*indexEnd != ' ' && *indexEnd != '\n' && *indexEnd != '/' && *wordEnd!= 0) { ++indexEnd; }
                if (indexStart != indexEnd) {
                    int index = stringToInt(indexStart, indexEnd - 1) - 1;
                    float *normalsPointer = (float *)loadedMesh->normals.values + (normalIndex * 3);
                    normalsPointer[0] = tempNormals[index][0];
                    normalsPointer[1] = tempNormals[index][1];
                    normalsPointer[2] = tempNormals[index][2];
                    ++normalIndex;
                }

                ((int *)(loadedMesh->indices.values))[indexIndex] = indexIndex;
                ++indexIndex;
            }
        }

        start = end;
        if (*start == 0) {
            break;
        }
        else {
            start++;
        }
    }
    //for (int i = 0; i < (int)numTriangles; ++i) {
    //    int temp = ((int *)loadedMesh->indices.values)[i*3+1];
    //    ((int *)loadedMesh->indices.values)[i*3+1] = ((int *)loadedMesh->indices.values)[i*3+2];
    //    ((int *)loadedMesh->indices.values)[i*3+2] = temp;
    //}
}

// parses .obj but also makes a copy of some of the data to use for level data
static void parseLevelOBJ (void *objData, game_assets *assets, int meshKey, int levelMeshKey,
                           memory_arena *workingMemory) 
{
    parseOBJ(objData, assets, meshKey, workingMemory);

    int numLevelMeshes = assets->numLevelMeshes;
    assert(numLevelMeshes < MAX_NUM_LEVEL_MESHES);

    level_mesh *levelMesh = (level_mesh *)allocateMemorySize(&assets->assetMemory, sizeof(level_mesh));
    assets->levelMeshes[numLevelMeshes] = levelMesh;
    assets->numLevelMeshes++;

    levelMesh->key = (level_mesh_key)levelMeshKey;

    loaded_mesh_asset *meshAssetData = (loaded_mesh_asset *)workingMemory->base;

    // NOTE(ebuchholz): assuming every index is unique and every 3 of them for a triangle, so
    // we can just loop through these positions and make a triangle for every 9 (3 vertices)
    float_mesh_attribute *positions = &meshAssetData->positions;
    int numPositions = positions->count;
    levelMesh->triangleCount = numPositions / 9;
    levelMesh->triangles = (triangle *)allocateMemorySize(&assets->assetMemory, sizeof(triangle) * levelMesh->triangleCount);
    levelMesh->boundingBox = {}; 
    int numTriangles = 0;
    for (int i = 0; i < numPositions; i += 9) {
        triangle *tri = levelMesh->triangles + numTriangles;
        tri->p0.x = ((float *)positions->values)[i];
        tri->p0.y = ((float *)positions->values)[i+1];
        tri->p0.z = ((float *)positions->values)[i+2];

        tri->p1.x = ((float *)positions->values)[i+3];
        tri->p1.y = ((float *)positions->values)[i+4];
        tri->p1.z = ((float *)positions->values)[i+5];

        tri->p2.x = ((float *)positions->values)[i+6];
        tri->p2.y = ((float *)positions->values)[i+7];
        tri->p2.z = ((float *)positions->values)[i+8];

        // TODO(ebuchholz): probably better just to keep track of the min/max coordinates, but
        // I already have the functions for these so it's easier
        aabb triangleAABB = getTriangleBounds(tri->p0, tri->p1, tri->p2);
        levelMesh->boundingBox = unionAABB(levelMesh->boundingBox, triangleAABB);

        numTriangles++;
    }
}

static void parseBitmap (void *fileData, game_assets *assets, int key, memory_arena *workingMemory) {
    int numTextures = assets->numTextures;
    assert(numTextures < MAX_NUM_TEXTURES);

    texture_asset *textureAsset = (texture_asset *)allocateMemorySize(&assets->assetMemory, sizeof(texture_asset *));
    assets->textures[numTextures] = textureAsset;
    assets->numTextures++;

    loaded_texture_asset *loadedBitmap = (loaded_texture_asset *)allocateMemorySize(workingMemory, sizeof(loaded_texture_asset));
    loadedBitmap->key = key;

    bitmap_header *header = (bitmap_header *)fileData;    
    unsigned char *pixels = (unsigned char *)((char *)fileData + header->bitmapOffset);

    int width = header->width;
    int height = header->height;
    loadedBitmap->width = width;
    loadedBitmap->height = height;
    unsigned char *bitmapPixels = (unsigned char *)allocateMemorySize(workingMemory, sizeof(unsigned char) * 4 * width * height);
    loadedBitmap->pixels = (unsigned int *)bitmapPixels;

    unsigned int numPixelValues = width * height * 3;
    unsigned int numBitmapValues = 0;
    for (unsigned int i = 0; i < numPixelValues; i += 3) {
        bitmapPixels[numBitmapValues] = pixels[i+2];
        bitmapPixels[numBitmapValues+1] = pixels[i+1];
        bitmapPixels[numBitmapValues+2] = pixels[i];
        bitmapPixels[numBitmapValues+3] = 0xff;
        numBitmapValues += 4;
    }
}

static void pushAsset (asset_list *assetList, char *path, asset_type type, int key1, int key2) {
    assert(assetList->numAssetsToLoad < assetList->maxAssetsToLoad);
    asset_to_load *assetToLoad = assetList->assetsToLoad + assetList->numAssetsToLoad;
    assetList->numAssetsToLoad++;
    assetToLoad->path = path;
    assetToLoad->type = type;
    assetToLoad->key1 = key1;
    assetToLoad->key2 = key2;
}

static void pushAsset (asset_list *assetList, char *path, asset_type type, int key) {
    pushAsset(assetList, path, type, key, -1);
}

// TODO(ebuchholz): Maybe pack everything into a single file and load that?
extern "C" void getGameAssetList (asset_list *assetList) {
    pushAsset(assetList, "assets/meshes/geosphere.obj", ASSET_TYPE_OBJ, MESH_KEY_SPHERE);
    pushAsset(assetList, "assets/meshes/cube.obj", ASSET_TYPE_OBJ, MESH_KEY_CUBE);
    pushAsset(assetList, "assets/meshes/cylinder.obj", ASSET_TYPE_OBJ, MESH_KEY_CYLINDER);
    pushAsset(assetList, "assets/textures/uv_test.bmp", ASSET_TYPE_BMP, TEXTURE_KEY_UV_TEST);
    pushAsset(assetList, "assets/textures/ground.bmp", ASSET_TYPE_BMP, TEXTURE_KEY_GROUND);
    pushAsset(assetList, "assets/textures/grey.bmp", ASSET_TYPE_BMP, TEXTURE_KEY_GREY);
    pushAsset(assetList, "assets/textures/blue.bmp", ASSET_TYPE_BMP, TEXTURE_KEY_BLUE);

    pushAsset(assetList, "assets/meshes/test_ground.obj", ASSET_TYPE_LEVEL_OBJ, 
              MESH_KEY_TEST_GROUND, LEVEL_MESH_KEY_TEST_GROUND);
}

extern "C" void parseGameAsset (void *assetData, asset_type type, int key1, int key2,
                                game_memory *gameMemory, memory_arena *workingMemory) 
{
    game_state *gameState = (game_state *)gameMemory->memory;
    if (!gameState->assetsInitialized) {
        gameState->assetsInitialized = true;

        gameState->memory = {};

        gameState->memory.size = 0;
        gameState->memory.capacity = gameMemory->memoryCapacity - sizeof(game_state);
        gameState->memory.base = (char *)gameMemory->memory + sizeof(game_state);

        gameState->assets = {};
        game_assets *assets = &gameState->assets;
        assets->assetMemory = {};
        assets->assetMemory.size = 0;
        assets->assetMemory.capacity = 1 * 1024 * 1024; // 1MB of asset data???
        assets->assetMemory.base = allocateMemorySize(&gameState->memory, assets->assetMemory.capacity); 
        assets->numMeshes = 0;
    } 
    // parse data
    // create/copy stuff into game memory
    // place whatever theplatform needs at the beginningof working memory
    switch (type) {
    default:
        assert(false); // must provide a valid type
        break;
    case ASSET_TYPE_OBJ:
        parseOBJ(assetData, &gameState->assets, key1, workingMemory);
        break;
    case ASSET_TYPE_LEVEL_OBJ:
        parseLevelOBJ(assetData, &gameState->assets, key1, key2, workingMemory);
        break;
    case ASSET_TYPE_BMP:
        parseBitmap(assetData, &gameState->assets, key1, workingMemory);
        break;
    }
}

static void drawModel (mesh_key meshKey, texture_key textureKey, 
                       matrix4x4 modelMatrix, render_command_list *renderCommands) {
    render_command_model *modelCommand = 
        (render_command_model *)pushRenderCommand(renderCommands,
                                                 RENDER_COMMAND_MODEL,
                                                 sizeof(render_command_model));
    modelCommand->meshKey = meshKey;
    modelCommand->textureKey = textureKey;
    modelCommand->modelMatrix = modelMatrix;
}

static void debugPlayerMovement (game_state *gameState, game_input *input) {
    const float PLAYER_SPEED = 1.0f;

    // Position
    if (input->forwardButton) {
        gameState->playerPos.z -= PLAYER_SPEED * DELTA_TIME;
    }
    if (input->backButton) {
        gameState->playerPos.z += PLAYER_SPEED * DELTA_TIME;
    }
    if (input->leftButton) {
        gameState->playerPos.x -= PLAYER_SPEED * DELTA_TIME;
    }
    if (input->rightButton) {
        gameState->playerPos.x += PLAYER_SPEED * DELTA_TIME;
    }
    if (input->upButton) {
        gameState->playerPos.y += PLAYER_SPEED * DELTA_TIME;
    }
    if (input->downButton) {
        gameState->playerPos.y -= PLAYER_SPEED * DELTA_TIME;
    }

    aabb *playerAABB = &gameState->playerAABB;
    playerAABB->min.x = gameState->playerPos.x - 0.5f;
    playerAABB->min.y = gameState->playerPos.y - 0.0f;
    playerAABB->min.z = gameState->playerPos.z - 0.5f;
    playerAABB->max.x = gameState->playerPos.x + 0.5f;
    playerAABB->max.y = gameState->playerPos.y + 1.0f;
    playerAABB->max.z = gameState->playerPos.z + 0.5f;
}

static void debugCameraMovement (debug_camera *debugCamera, game_input *input) {
    const float CAMERA_SPEED = 3.0f;
    const float CAMERA_TURN_SPEED = 1.0f;

    // Position
    vector3 moveVector = {};
    if (input->turnUpButton) {
        moveVector.z -= CAMERA_SPEED * DELTA_TIME;
    }
    if (input->turnDownButton) {
        moveVector.z += CAMERA_SPEED * DELTA_TIME;
    }
    if (input->turnLeftButton) {
        moveVector.x -= CAMERA_SPEED * DELTA_TIME;
    }
    if (input->turnRightButton) {
        moveVector.x += CAMERA_SPEED * DELTA_TIME;
    }

    // Rotation
    if (input->pointerJustDown) {
        debugCamera->lastPointerX = input->pointerX;
        debugCamera->lastPointerY = input->pointerY;
    }
    if (input->pointerDown) {
        int pointerDX = input->pointerX - debugCamera->lastPointerX;
        int pointerDY = input->pointerY - debugCamera->lastPointerY;

        float yaw = (float)pointerDX * 0.25f;
        float pitch = (float)pointerDY * 0.25f;

        debugCamera->rotation = quaternionFromAxisAngle(Vector3(0, 1, 0), -yaw * DELTA_TIME) * (debugCamera->rotation);
        debugCamera->rotation = (debugCamera->rotation) * quaternionFromAxisAngle(Vector3(1, 0, 0), -pitch * DELTA_TIME);

        debugCamera->lastPointerX = input->pointerX;
        debugCamera->lastPointerY = input->pointerY;
    }
    // Move in the direction of the current rotation
    moveVector = rotateVectorByQuaternion(moveVector, debugCamera->rotation);
    debugCamera->pos += moveVector;
}

extern "C" void updateGame (game_input *input, game_memory *gameMemory, render_command_list *renderCommands) { 
    game_state *gameState = (game_state *)gameMemory->memory;
    if (!gameState->gameInitialized) {
        gameState->gameInitialized = true;

        debug_camera *debugCamera = &gameState->debugCamera;
        debugCamera->pos = {};
        debugCamera->pos.x = 3.0f;
        debugCamera->pos.y = 3.0f;
        debugCamera->pos.z = 3.0f;
        debugCamera->rotation = 
            quaternionFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 45.0f * (PI / 180.0f)) *
            quaternionFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), -33.0f * (PI / 180.0f));
        debugCamera->lastPointerX = 0;
        debugCamera->lastPointerY = 0;

        gameState->playerPos = {};
    }

    debugPlayerMovement(gameState, input);
    debugCameraMovement(&gameState->debugCamera, input);

    // TODO(ebuchholz): get screen dimensions from render commands? and use them
    matrix4x4 projMatrix = createPerspectiveMatrix(0.1f, 1000.0f, (16.0f / 9.0f), 80.0f);
    matrix4x4 viewMatrix = createViewMatrix(gameState->debugCamera.rotation, 
                                            gameState->debugCamera.pos.x,
                                            gameState->debugCamera.pos.y,
                                            gameState->debugCamera.pos.z);

    render_command_set_camera *setCameraCommand = 
        (render_command_set_camera *)pushRenderCommand(renderCommands,
                                                 RENDER_COMMAND_SET_CAMERA,
                                                 sizeof(render_command_set_camera));
    setCameraCommand->viewMatrix = viewMatrix;
    setCameraCommand->projMatrix = projMatrix;

    texture_key cylKey = 
        aabbIntersection(gameState->playerAABB, 
                         gameState->assets.levelMeshes[LEVEL_MESH_KEY_TEST_GROUND]->boundingBox) ? TEXTURE_KEY_BLUE : TEXTURE_KEY_GREY;
    matrix4x4 modelMatrix = translationMatrix(gameState->playerPos.x, gameState->playerPos.y, gameState->playerPos.z);
    drawModel(MESH_KEY_CYLINDER, cylKey, modelMatrix, renderCommands);

    modelMatrix = translationMatrix(0.0f, 0.0f, 0.0f) * scaleMatrix(1.0f);
    drawModel(MESH_KEY_TEST_GROUND, TEXTURE_KEY_GREY, modelMatrix, renderCommands);
}
