// WebIDL bindings
#include "../../game/run_around_platform.h"

#include "bindings.cpp"

// addition embind bindings
#include <emscripten/bind.h>

using namespace emscripten;

int sizeof_matrix3x3 () {
    return sizeof(matrix3x3);
}  
int sizeof_matrix4x4 () {
    return sizeof(matrix4x4);
}  
int sizeof_vector3 () {
    return sizeof(vector3);
}  
int sizeof_quaternion () {
    return sizeof(quaternion);
}  
int sizeof_aabb () {
    return sizeof(aabb);
}  
int sizeof_sphere () {
    return sizeof(sphere);
}  
int sizeof_plane () {
    return sizeof(plane);
}  
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
int sizeof_render_mesh_command () {
    return sizeof(render_mesh_command);
}  
int sizeof_render_command_set_camera () {
    return sizeof(render_command_set_camera);
}
int sizeof_render_command_list () {
    return sizeof(render_command_list);
}  
int sizeof_game_input () {
    return sizeof(game_input);
}  
int sizeof_game_memory () {
    return sizeof(game_memory);
}  

EMSCRIPTEN_BINDINGS(bindings) {
    function ("sizeof_matrix3x3", &sizeof_matrix3x3);
    function ("sizeof_matrix4x4", &sizeof_matrix4x4);
    function ("sizeof_vector3", &sizeof_vector3);
    function ("sizeof_quaternion", &sizeof_quaternion);
    function ("sizeof_aabb", &sizeof_aabb);
    function ("sizeof_sphere", &sizeof_sphere);
    function ("sizeof_plane", &sizeof_plane);
    function ("sizeof_asset_to_load", &sizeof_asset_to_load);
    function ("sizeof_asset_list", &sizeof_asset_list);
    function ("sizeof_float_mesh_attribute", &sizeof_float_mesh_attribute);
    function ("sizeof_int_mesh_attribute", &sizeof_int_mesh_attribute);
    function ("sizeof_loaded_mesh_asset", &sizeof_loaded_mesh_asset);
    function ("sizeof_memory_arena", &sizeof_memory_arena);
    function ("sizeof_render_command_header", &sizeof_render_command_header);
    function ("sizeof_render_mesh_command", &sizeof_render_mesh_command);
    function ("sizeof_render_command_set_camera", &sizeof_render_command_set_camera);
    function ("sizeof_render_command_list", &sizeof_render_command_list);
    function ("sizeof_game_input", &sizeof_game_input);
    function ("sizeof_game_memory", &sizeof_game_memory);
}

