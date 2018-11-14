#include "run_around_game.h"

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

static float stringToFloat (char *string) {
    char *end = string;
    while (*end != 0) {
        ++end;
    }
    --end;
    return stringToFloat(string, end);
}

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

    float t = stringToFloat("234234.343");

}

static void pushAsset (asset_list *assetList, char *path, asset_type type, int key) {
    assert(assetList->numAssetsToLoad < assetList->maxAssetsToLoad);
    asset_to_load *assetToLoad = assetList->assetsToLoad + assetList->numAssetsToLoad;
    assetList->numAssetsToLoad++;
    assetToLoad->path = path;
    assetToLoad->type = type;
    assetToLoad->key = key;
}

// TODO(ebuchholz): Maybe pack everything into a single file and load that?
extern "C" void getGameAssetList (asset_list *assetList) {
    pushAsset(assetList, "assets/meshes/geosphere.obj", ASSET_TYPE_OBJ, MESH_KEY_SPHERE);
    pushAsset(assetList, "assets/meshes/simplebunny.obj", ASSET_TYPE_OBJ, MESH_KEY_TREE);
}

extern "C" void parseGameAsset (void *assetData, asset_type type, int key,
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
        parseOBJ(assetData, &gameState->assets, key, workingMemory);
        break;
    }
}

extern "C" void updateGame (render_command_list *renderCommands) { 
    render_mesh_command *meshCommand = 
        (render_mesh_command *)pushRenderCommand(renderCommands,
                                                 RENDER_COMMAND_MESH,
                                                 sizeof(render_mesh_command));
    meshCommand->key = MESH_KEY_SPHERE;

    meshCommand = (render_mesh_command *)pushRenderCommand(renderCommands,
                                                           RENDER_COMMAND_MESH,
                                                           sizeof(render_mesh_command));
    meshCommand->key = MESH_KEY_TREE;
}
