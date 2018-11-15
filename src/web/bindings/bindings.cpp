
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

// render_command_set_camera

matrix4x4* EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_set_camera_get_viewMatrix_0(render_command_set_camera* self) {
  return &self->viewMatrix;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_set_camera_set_viewMatrix_1(render_command_set_camera* self, matrix4x4* arg0) {
  self->viewMatrix = *arg0;
}

matrix4x4* EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_set_camera_get_projMatrix_0(render_command_set_camera* self) {
  return &self->projMatrix;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_set_camera_set_projMatrix_1(render_command_set_camera* self, matrix4x4* arg0) {
  self->projMatrix = *arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_command_set_camera___destroy___0(render_command_set_camera* self) {
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

// aabb

vector3* EMSCRIPTEN_KEEPALIVE emscripten_bind_aabb_get_min_0(aabb* self) {
  return &self->min;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_aabb_set_min_1(aabb* self, vector3* arg0) {
  self->min = *arg0;
}

vector3* EMSCRIPTEN_KEEPALIVE emscripten_bind_aabb_get_max_0(aabb* self) {
  return &self->max;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_aabb_set_max_1(aabb* self, vector3* arg0) {
  self->max = *arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_aabb___destroy___0(aabb* self) {
  delete self;
}

// vector3

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_x_0(vector3* self) {
  return self->x;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_x_1(vector3* self, float arg0) {
  self->x = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_y_0(vector3* self) {
  return self->y;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_y_1(vector3* self, float arg0) {
  self->y = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_z_0(vector3* self) {
  return self->z;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_z_1(vector3* self, float arg0) {
  self->z = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_r_0(vector3* self) {
  return self->r;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_r_1(vector3* self, float arg0) {
  self->r = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_g_0(vector3* self) {
  return self->g;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_g_1(vector3* self, float arg0) {
  self->g = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_b_0(vector3* self) {
  return self->b;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_b_1(vector3* self, float arg0) {
  self->b = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_get_c_1(vector3* self, int arg0) {
  return self->c[arg0];
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3_set_c_2(vector3* self, int arg0, float arg1) {
  self->c[arg0] = arg1;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_vector3___destroy___0(vector3* self) {
  delete self;
}

// game_input

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_forwardButton_0(game_input* self) {
  return self->forwardButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_forwardButton_1(game_input* self, bool arg0) {
  self->forwardButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_backButton_0(game_input* self) {
  return self->backButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_backButton_1(game_input* self, bool arg0) {
  self->backButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_leftButton_0(game_input* self) {
  return self->leftButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_leftButton_1(game_input* self, bool arg0) {
  self->leftButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_rightButton_0(game_input* self) {
  return self->rightButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_rightButton_1(game_input* self, bool arg0) {
  self->rightButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_upButton_0(game_input* self) {
  return self->upButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_upButton_1(game_input* self, bool arg0) {
  self->upButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_downButton_0(game_input* self) {
  return self->downButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_downButton_1(game_input* self, bool arg0) {
  self->downButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_turnUpButton_0(game_input* self) {
  return self->turnUpButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_turnUpButton_1(game_input* self, bool arg0) {
  self->turnUpButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_turnDownButton_0(game_input* self) {
  return self->turnDownButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_turnDownButton_1(game_input* self, bool arg0) {
  self->turnDownButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_turnLeftButton_0(game_input* self) {
  return self->turnLeftButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_turnLeftButton_1(game_input* self, bool arg0) {
  self->turnLeftButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_turnRightButton_0(game_input* self) {
  return self->turnRightButton;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_turnRightButton_1(game_input* self, bool arg0) {
  self->turnRightButton = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_pointerDown_0(game_input* self) {
  return self->pointerDown;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_pointerDown_1(game_input* self, bool arg0) {
  self->pointerDown = arg0;
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_pointerJustDown_0(game_input* self) {
  return self->pointerJustDown;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_pointerJustDown_1(game_input* self, bool arg0) {
  self->pointerJustDown = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_pointerX_0(game_input* self) {
  return self->pointerX;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_pointerX_1(game_input* self, int arg0) {
  self->pointerX = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_get_pointerY_0(game_input* self) {
  return self->pointerY;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input_set_pointerY_1(game_input* self, int arg0) {
  self->pointerY = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_game_input___destroy___0(game_input* self) {
  delete self;
}

// matrix4x4

float EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix4x4_get_m_1(matrix4x4* self, int arg0) {
  return self->m[arg0];
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix4x4_set_m_2(matrix4x4* self, int arg0, float arg1) {
  self->m[arg0] = arg1;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix4x4___destroy___0(matrix4x4* self) {
  delete self;
}

// sphere

vector3* EMSCRIPTEN_KEEPALIVE emscripten_bind_sphere_get_pos_0(sphere* self) {
  return &self->pos;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_sphere_set_pos_1(sphere* self, vector3* arg0) {
  self->pos = *arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_sphere_get_radius_0(sphere* self) {
  return self->radius;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_sphere_set_radius_1(sphere* self, float arg0) {
  self->radius = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_sphere___destroy___0(sphere* self) {
  delete self;
}

// quaternion

float EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_get_w_0(quaternion* self) {
  return self->w;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_set_w_1(quaternion* self, float arg0) {
  self->w = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_get_x_0(quaternion* self) {
  return self->x;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_set_x_1(quaternion* self, float arg0) {
  self->x = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_get_y_0(quaternion* self) {
  return self->y;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_set_y_1(quaternion* self, float arg0) {
  self->y = arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_get_z_0(quaternion* self) {
  return self->z;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion_set_z_1(quaternion* self, float arg0) {
  self->z = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_quaternion___destroy___0(quaternion* self) {
  delete self;
}

// plane

vector3* EMSCRIPTEN_KEEPALIVE emscripten_bind_plane_get_normal_0(plane* self) {
  return &self->normal;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_plane_set_normal_1(plane* self, vector3* arg0) {
  self->normal = *arg0;
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_plane_get_distance_0(plane* self) {
  return self->distance;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_plane_set_distance_1(plane* self, float arg0) {
  self->distance = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_plane___destroy___0(plane* self) {
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

// render_mesh_command

int EMSCRIPTEN_KEEPALIVE emscripten_bind_render_mesh_command_get_key_0(render_mesh_command* self) {
  return self->key;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_mesh_command_set_key_1(render_mesh_command* self, int arg0) {
  self->key = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_render_mesh_command___destroy___0(render_mesh_command* self) {
  delete self;
}

// matrix3x3

float EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix3x3_get_m_1(matrix3x3* self, int arg0) {
  return self->m[arg0];
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix3x3_set_m_2(matrix3x3* self, int arg0, float arg1) {
  self->m[arg0] = arg1;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_matrix3x3___destroy___0(matrix3x3* self) {
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
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_MESH() {
  return RENDER_COMMAND_MESH;
}
render_command_type EMSCRIPTEN_KEEPALIVE emscripten_enum_render_command_type_RENDER_COMMAND_SET_CAMERA() {
  return RENDER_COMMAND_SET_CAMERA;
}

}

