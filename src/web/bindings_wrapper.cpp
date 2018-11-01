// WebIDL bindings
#include "../game/run_around_platform.h"

#include "bindings.cpp"

// addition embind bindings
#include <emscripten/bind.h>

using namespace emscripten;

int sizeof_rectangle_list () {
    return sizeof(rectangle_list);
}  
int sizeof_colored_rectangle () {
    return sizeof(colored_rectangle);   
}
int sizeof_image_asset () {
    return sizeof(image_asset);   
}
int sizeof_font_asset () {
    return sizeof(font_asset);   
}
int sizeof_asset_to_load () {
    return sizeof(asset_to_load);   
}
int sizeof_asset_list () {
    return sizeof(asset_list);   
}

EMSCRIPTEN_BINDINGS(bindings) {
    function ("sizeof_rectangle_list", &sizeof_rectangle_list);
    function ("sizeof_colored_rectangle", &sizeof_colored_rectangle);
    function ("sizeof_image_asset", &sizeof_image_asset);
    function ("sizeof_font_asset", &sizeof_font_asset);
    function ("sizeof_asset_to_load", &sizeof_asset_to_load);
    function ("sizeof_asset_list", &sizeof_asset_list);
}

