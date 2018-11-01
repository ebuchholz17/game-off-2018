#ifndef RUN_AROUND_PLATFORM_H
#define RUN_AROUND_PLATFORM_H

enum asset_type {
    ASSET_TYPE_IMAGE,
    ASSET_TYPE_FONT
};

struct image_asset {
    unsigned int id;
    char *path;
};

struct font_asset {
    unsigned int id;
    char *name;
    char *path;
};

struct asset_to_load {
    asset_type type;
    union {
        image_asset image;
        font_asset font;
    };
};

struct asset_list {
    asset_to_load *assetsToLoad;
    unsigned int numAssets;
    unsigned int maxAssets;
};

struct colored_rectangle {
    int x;
    int y;
    int width;
    int height;
    unsigned int color;
};

struct rectangle_list {
    int numRectangles;
    int maxRectangles;
    colored_rectangle *rectangles;
};

#endif
