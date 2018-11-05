// WebIDL bindings
#include "../../game/run_around_platform.h"

#include "bindings.cpp"

// addition embind bindings
#include <emscripten/bind.h>

using namespace emscripten;

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

EMSCRIPTEN_BINDINGS(bindings) {
    function ("sizeof_memory_arena", &sizeof_memory_arena);
    function ("sizeof_render_command_header", &sizeof_render_command_header);
    function ("sizeof_render_rectangle_command", &sizeof_render_rectangle_command);
    function ("sizeof_render_horizontal_line_command", &sizeof_render_horizontal_line_command);
    function ("sizeof_render_command_list", &sizeof_render_command_list);
}

