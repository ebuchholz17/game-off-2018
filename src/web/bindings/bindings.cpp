
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.
void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    EM_ASM({
      throw 'Array index ' + $0 + ' out of bounds: [0,' + $1 + ')';
    }, array_idx, array_size);
  }
}

// render_rectangle_command

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_get_x_0(render_rectangle_command* self) {
  return self->x;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_set_x_1(render_rectangle_command* self, unsigned int arg0) {
  self->x = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_get_y_0(render_rectangle_command* self) {
  return self->y;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_set_y_1(render_rectangle_command* self, unsigned int arg0) {
  self->y = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_get_width_0(render_rectangle_command* self) {
  return self->width;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_set_width_1(render_rectangle_command* self, unsigned int arg0) {
  self->width = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_get_height_0(render_rectangle_command* self) {
  return self->height;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_set_height_1(render_rectangle_command* self, unsigned int arg0) {
  self->height = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_get_color_0(render_rectangle_command* self) {
  return self->color;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command_set_color_1(render_rectangle_command* self, unsigned int arg0) {
  self->color = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_rectangle_command___destroy___0(render_rectangle_command* self) {
  delete self;
}

// memory_arena

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_get_base_0(memory_arena* self) {
  return self->base;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_set_base_1(memory_arena* self, void* arg0) {
  self->base = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_get_size_0(memory_arena* self) {
  return self->size;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_set_size_1(memory_arena* self, unsigned int arg0) {
  self->size = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_get_capacity_0(memory_arena* self) {
  return self->capacity;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena_set_capacity_1(memory_arena* self, unsigned int arg0) {
  self->capacity = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_memory_arena___destroy___0(memory_arena* self) {
  delete self;
}

// render_command_header

render_command_type EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_header_get_type_0(render_command_header* self) {
  return self->type;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_header_set_type_1(render_command_header* self, render_command_type arg0) {
  self->type = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_header___destroy___0(render_command_header* self) {
  delete self;
}

// render_horizontal_line_command

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_horizontal_line_command_get_color_0(render_horizontal_line_command* self) {
  return self->color;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_horizontal_line_command_set_color_1(render_horizontal_line_command* self, unsigned int arg0) {
  self->color = arg0;
}

char EMSCRIPTEN_KEEPALIVE emscripten_bind_render_horizontal_line_command_get_lineNum_0(render_horizontal_line_command* self) {
  return self->lineNum;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_horizontal_line_command_set_lineNum_1(render_horizontal_line_command* self, char arg0) {
  self->lineNum = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_horizontal_line_command___destroy___0(render_horizontal_line_command* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// render_command_list

memory_arena* EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_list_get_memory_0(render_command_list* self) {
  return &self->memory;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_list_set_memory_1(render_command_list* self, memory_arena* arg0) {
  self->memory = *arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_list___destroy___0(render_command_list* self) {
  delete self;
}

// render_command_type
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_RECTANGLE() {
  return RENDER_COMMAND_RECTANGLE;
}
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_HORIZONTAL_LINE() {
  return RENDER_COMMAND_HORIZONTAL_LINE;
}

}

