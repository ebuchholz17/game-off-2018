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

EMSCRIPTEN_BINDINGS(bindings) {
    function ("sizeof_rectangle_list", &sizeof_rectangle_list);
    function ("sizeof_colored_rectangle", &sizeof_colored_rectangle);
}

