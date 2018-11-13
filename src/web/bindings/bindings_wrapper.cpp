// WebIDL bindings
#include "../../game/run_around_platform.h"

#include "bindings.cpp"

// addition embind bindings
#include <emscripten/bind.h>

using namespace emscripten;

int sizeof_asset_to_load () {
    return sizeof(asset_to_load);
}  
int sizeof_asset_list () {
    return sizeof(asset_list);
}  
int sizeof_float_mesh_attribute () {
    return sizeof(float_mesh_attribute);
}  
int sizeof_int_mesh_attribute () {
    return sizeof(int_mesh_attribute);
}  
int sizeof_loaded_mesh_asset () {
    return sizeof(loaded_mesh_asset);
}  
int sizeof_memory_arena () {
    return sizeof(memory_arena);
}  
int sizeof_render_command_header () {
    return sizeof(render_command_header);
}  
int sizeof_render_rectangle_command () {
    return sizeof(render_rectangle_command);
}  
int sizeof_render_horizontal_line_command () {
    return sizeof(render_horizontal_line_command);
}  
int sizeof_render_command_list () {
    return sizeof(render_command_list);
}  
int sizeof_game_memory () {
    return sizeof(game_memory);
}  

EMSCRIPTEN_BINDINGS(bindings) {
    function ("sizeof_asset_to_load", &sizeof_asset_to_load);
    function ("sizeof_asset_list", &sizeof_asset_list);
    function ("sizeof_float_mesh_attribute", &sizeof_float_mesh_attribute);
    function ("sizeof_int_mesh_attribute", &sizeof_int_mesh_attribute);
    function ("sizeof_loaded_mesh_asset", &sizeof_loaded_mesh_asset);
    function ("sizeof_memory_arena", &sizeof_memory_arena);
    function ("sizeof_render_command_header", &sizeof_render_command_header);
    function ("sizeof_render_rectangle_command", &sizeof_render_rectangle_command);
    function ("sizeof_render_horizontal_line_command", &sizeof_render_horizontal_line_command);
    function ("sizeof_render_command_list", &sizeof_render_command_list);
    function ("sizeof_game_memory", &sizeof_game_memory);
}

