
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

// loaded_mesh_asset

int EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_get_key_0(loaded_mesh_asset* self) {
  return self->key;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_set_key_1(loaded_mesh_asset* self, int arg0) {
  self->key = arg0;
}

float_mesh_attribute* EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_get_positions_0(loaded_mesh_asset* self) {
  return &self->positions;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_set_positions_1(loaded_mesh_asset* self, float_mesh_attribute* arg0) {
  self->positions = *arg0;
}

float_mesh_attribute* EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_get_texCoords_0(loaded_mesh_asset* self) {
  return &self->texCoords;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_set_texCoords_1(loaded_mesh_asset* self, float_mesh_attribute* arg0) {
  self->texCoords = *arg0;
}

float_mesh_attribute* EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_get_normals_0(loaded_mesh_asset* self) {
  return &self->normals;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_set_normals_1(loaded_mesh_asset* self, float_mesh_attribute* arg0) {
  self->normals = *arg0;
}

int_mesh_attribute* EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_get_indices_0(loaded_mesh_asset* self) {
  return &self->indices;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset_set_indices_1(loaded_mesh_asset* self, int_mesh_attribute* arg0) {
  self->indices = *arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_loaded_mesh_asset___destroy___0(loaded_mesh_asset* self) {
  delete self;
}

// game_memory

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_game_memory_get_memory_0(game_memory* self) {
  return self->memory;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_memory_set_memory_1(game_memory* self, void* arg0) {
  self->memory = arg0;
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_game_memory_get_memoryCapacity_0(game_memory* self) {
  return self->memoryCapacity;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_memory_set_memoryCapacity_1(game_memory* self, unsigned int arg0) {
  self->memoryCapacity = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_memory___destroy___0(game_memory* self) {
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

// float_mesh_attribute

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_float_mesh_attribute_get_values_0(float_mesh_attribute* self) {
  return self->values;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_float_mesh_attribute_set_values_1(float_mesh_attribute* self, void* arg0) {
  self->values = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_float_mesh_attribute_get_count_0(float_mesh_attribute* self) {
  return self->count;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_float_mesh_attribute_set_count_1(float_mesh_attribute* self, int arg0) {
  self->count = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_float_mesh_attribute___destroy___0(float_mesh_attribute* self) {
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

// asset_list

int EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_numAssetsToLoad_0(asset_list* self) {
  return self->numAssetsToLoad;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_numAssetsToLoad_1(asset_list* self, int arg0) {
  self->numAssetsToLoad = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_maxAssetsToLoad_0(asset_list* self) {
  return self->maxAssetsToLoad;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_maxAssetsToLoad_1(asset_list* self, int arg0) {
  self->maxAssetsToLoad = arg0;
}

asset_to_load* EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_get_assetsToLoad_0(asset_list* self) {
  return self->assetsToLoad;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list_set_assetsToLoad_1(asset_list* self, asset_to_load* arg0) {
  self->assetsToLoad = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_list___destroy___0(asset_list* self) {
  delete self;
}

// asset_to_load

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_path_0(asset_to_load* self) {
  return self->path;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_path_1(asset_to_load* self, char* arg0) {
  self->path = arg0;
}

asset_type EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_type_0(asset_to_load* self) {
  return self->type;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_type_1(asset_to_load* self, asset_type arg0) {
  self->type = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_get_key_0(asset_to_load* self) {
  return self->key;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load_set_key_1(asset_to_load* self, int arg0) {
  self->key = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_asset_to_load___destroy___0(asset_to_load* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// int_mesh_attribute

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_int_mesh_attribute_get_values_0(int_mesh_attribute* self) {
  return self->values;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_int_mesh_attribute_set_values_1(int_mesh_attribute* self, void* arg0) {
  self->values = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_int_mesh_attribute_get_count_0(int_mesh_attribute* self) {
  return self->count;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_int_mesh_attribute_set_count_1(int_mesh_attribute* self, int arg0) {
  self->count = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_int_mesh_attribute___destroy___0(int_mesh_attribute* self) {
  delete self;
}

// asset_type
asset_type EMSCRIPTEN_KEEPALIVE emscripten_enum_asset_type_ASSET_TYPE_OBJ() {
  return ASSET_TYPE_OBJ;
}

// render_command_type
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_RECTANGLE() {
  return RENDER_COMMAND_RECTANGLE;
}
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_HORIZONTAL_LINE() {
  return RENDER_COMMAND_HORIZONTAL_LINE;
}

}

