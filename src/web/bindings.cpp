
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

// font_asset

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_get_id_0(font_asset* self) {
  return self->id;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_set_id_1(font_asset* self, unsigned int arg0) {
  self->id = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_get_name_0(font_asset* self) {
  return self->name;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_set_name_1(font_asset* self, char* arg0) {
  self->name = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_get_path_0(font_asset* self) {
  return self->path;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset_set_path_1(font_asset* self, char* arg0) {
  self->path = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_font_asset___destroy___0(font_asset* self) {
  delete self;
}

// asset_list

asset_to_load* EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_assetsToLoad_0(asset_list* self) {
  return self->assetsToLoad;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_assetsToLoad_1(asset_list* self, asset_to_load* arg0) {
  self->assetsToLoad = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_numAssets_0(asset_list* self) {
  return self->numAssets;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_numAssets_1(asset_list* self, unsigned int arg0) {
  self->numAssets = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_maxAssets_0(asset_list* self) {
  return self->maxAssets;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_maxAssets_1(asset_list* self, unsigned int arg0) {
  self->maxAssets = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list___destroy___0(asset_list* self) {
  delete self;
}

// asset_to_load

asset_type EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_type_0(asset_to_load* self) {
  return self->type;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_type_1(asset_to_load* self, asset_type arg0) {
  self->type = arg0;
}

image_asset* EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_image_0(asset_to_load* self) {
  return &self->image;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_image_1(asset_to_load* self, image_asset* arg0) {
  self->image = *arg0;
}

font_asset* EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_font_0(asset_to_load* self) {
  return &self->font;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_font_1(asset_to_load* self, font_asset* arg0) {
  self->font = *arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load___destroy___0(asset_to_load* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// image_asset

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_image_asset_get_id_0(image_asset* self) {
  return self->id;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_image_asset_set_id_1(image_asset* self, unsigned int arg0) {
  self->id = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_image_asset_get_path_0(image_asset* self) {
  return self->path;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_image_asset_set_path_1(image_asset* self, char* arg0) {
  self->path = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_image_asset___destroy___0(image_asset* self) {
  delete self;
}

// asset_type
asset_type EMSCRIPTEN_KEEPALIVE emscripten_enum_asset_type_ASSET_TYPE_IMAGE() {
  return ASSET_TYPE_IMAGE;
}
asset_type EMSCRIPTEN_KEEPALIVE emscripten_enum_asset_type_ASSET_TYPE_FONT() {
  return ASSET_TYPE_FONT;
}

}

