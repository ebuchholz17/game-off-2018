
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

// rectangle_list

int EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_get_numRectangles_0(rectangle_list* self) {
  return self->numRectangles;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_set_numRectangles_1(rectangle_list* self, int arg0) {
  self->numRectangles = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_get_maxRectangles_0(rectangle_list* self) {
  return self->maxRectangles;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_set_maxRectangles_1(rectangle_list* self, int arg0) {
  self->maxRectangles = arg0;
}

colored_rectangle* EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_get_rectangles_0(rectangle_list* self) {
  return self->rectangles;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list_set_rectangles_1(rectangle_list* self, colored_rectangle* arg0) {
  self->rectangles = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_rectangle_list___destroy___0(rectangle_list* self) {
  delete self;
}

// colored_rectangle

int EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_get_x_0(colored_rectangle* self) {
  return self->x;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_set_x_1(colored_rectangle* self, int arg0) {
  self->x = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_get_y_0(colored_rectangle* self) {
  return self->y;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_set_y_1(colored_rectangle* self, int arg0) {
  self->y = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_get_width_0(colored_rectangle* self) {
  return self->width;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_set_width_1(colored_rectangle* self, int arg0) {
  self->width = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_get_height_0(colored_rectangle* self) {
  return self->height;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_set_height_1(colored_rectangle* self, int arg0) {
  self->height = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_get_color_0(colored_rectangle* self) {
  return self->color;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle_set_color_1(colored_rectangle* self, unsigned int arg0) {
  self->color = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_colored_rectangle___destroy___0(colored_rectangle* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

}

