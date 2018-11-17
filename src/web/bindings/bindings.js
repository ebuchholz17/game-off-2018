
// Bindings utilities

function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc: function(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy: function(array, view, offset) {
    var offsetShifted = offset;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offsetShifted >>= 1; break;
      case 4: offsetShifted >>= 2; break;
      case 8: offsetShifted >>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offsetShifted + i] = array[i];
    }
  },
};

function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// loaded_mesh_asset
/** @suppress {undefinedVars, duplicate} */function loaded_mesh_asset() { throw "cannot construct a loaded_mesh_asset, no constructor in IDL" }
loaded_mesh_asset.prototype = Object.create(WrapperObject.prototype);
loaded_mesh_asset.prototype.constructor = loaded_mesh_asset;
loaded_mesh_asset.prototype.__class__ = loaded_mesh_asset;
loaded_mesh_asset.__cache__ = {};
Module['loaded_mesh_asset'] = loaded_mesh_asset;

  loaded_mesh_asset.prototype['get_key'] = loaded_mesh_asset.prototype.get_key = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_loaded_mesh_asset_get_key_0(self);
};
    loaded_mesh_asset.prototype['set_key'] = loaded_mesh_asset.prototype.set_key = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_mesh_asset_set_key_1(self, arg0);
};
    Object.defineProperty(loaded_mesh_asset.prototype, 'key', { get: loaded_mesh_asset.prototype.get_key, set: loaded_mesh_asset.prototype.set_key }) 
  loaded_mesh_asset.prototype['get_positions'] = loaded_mesh_asset.prototype.get_positions = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_loaded_mesh_asset_get_positions_0(self), float_mesh_attribute);
};
    loaded_mesh_asset.prototype['set_positions'] = loaded_mesh_asset.prototype.set_positions = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_mesh_asset_set_positions_1(self, arg0);
};
    Object.defineProperty(loaded_mesh_asset.prototype, 'positions', { get: loaded_mesh_asset.prototype.get_positions, set: loaded_mesh_asset.prototype.set_positions }) 
  loaded_mesh_asset.prototype['get_texCoords'] = loaded_mesh_asset.prototype.get_texCoords = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_loaded_mesh_asset_get_texCoords_0(self), float_mesh_attribute);
};
    loaded_mesh_asset.prototype['set_texCoords'] = loaded_mesh_asset.prototype.set_texCoords = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_mesh_asset_set_texCoords_1(self, arg0);
};
    Object.defineProperty(loaded_mesh_asset.prototype, 'texCoords', { get: loaded_mesh_asset.prototype.get_texCoords, set: loaded_mesh_asset.prototype.set_texCoords }) 
  loaded_mesh_asset.prototype['get_normals'] = loaded_mesh_asset.prototype.get_normals = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_loaded_mesh_asset_get_normals_0(self), float_mesh_attribute);
};
    loaded_mesh_asset.prototype['set_normals'] = loaded_mesh_asset.prototype.set_normals = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_mesh_asset_set_normals_1(self, arg0);
};
    Object.defineProperty(loaded_mesh_asset.prototype, 'normals', { get: loaded_mesh_asset.prototype.get_normals, set: loaded_mesh_asset.prototype.set_normals }) 
  loaded_mesh_asset.prototype['get_indices'] = loaded_mesh_asset.prototype.get_indices = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_loaded_mesh_asset_get_indices_0(self), int_mesh_attribute);
};
    loaded_mesh_asset.prototype['set_indices'] = loaded_mesh_asset.prototype.set_indices = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_mesh_asset_set_indices_1(self, arg0);
};
    Object.defineProperty(loaded_mesh_asset.prototype, 'indices', { get: loaded_mesh_asset.prototype.get_indices, set: loaded_mesh_asset.prototype.set_indices }) 
  loaded_mesh_asset.prototype['__destroy__'] = loaded_mesh_asset.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_loaded_mesh_asset___destroy___0(self);
};
// float_mesh_attribute
/** @suppress {undefinedVars, duplicate} */function float_mesh_attribute() { throw "cannot construct a float_mesh_attribute, no constructor in IDL" }
float_mesh_attribute.prototype = Object.create(WrapperObject.prototype);
float_mesh_attribute.prototype.constructor = float_mesh_attribute;
float_mesh_attribute.prototype.__class__ = float_mesh_attribute;
float_mesh_attribute.__cache__ = {};
Module['float_mesh_attribute'] = float_mesh_attribute;

  float_mesh_attribute.prototype['get_values'] = float_mesh_attribute.prototype.get_values = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_float_mesh_attribute_get_values_0(self), VoidPtr);
};
    float_mesh_attribute.prototype['set_values'] = float_mesh_attribute.prototype.set_values = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_float_mesh_attribute_set_values_1(self, arg0);
};
    Object.defineProperty(float_mesh_attribute.prototype, 'values', { get: float_mesh_attribute.prototype.get_values, set: float_mesh_attribute.prototype.set_values }) 
  float_mesh_attribute.prototype['get_count'] = float_mesh_attribute.prototype.get_count = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_float_mesh_attribute_get_count_0(self);
};
    float_mesh_attribute.prototype['set_count'] = float_mesh_attribute.prototype.set_count = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_float_mesh_attribute_set_count_1(self, arg0);
};
    Object.defineProperty(float_mesh_attribute.prototype, 'count', { get: float_mesh_attribute.prototype.get_count, set: float_mesh_attribute.prototype.set_count }) 
  float_mesh_attribute.prototype['__destroy__'] = float_mesh_attribute.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_float_mesh_attribute___destroy___0(self);
};
// memory_arena
/** @suppress {undefinedVars, duplicate} */function memory_arena() { throw "cannot construct a memory_arena, no constructor in IDL" }
memory_arena.prototype = Object.create(WrapperObject.prototype);
memory_arena.prototype.constructor = memory_arena;
memory_arena.prototype.__class__ = memory_arena;
memory_arena.__cache__ = {};
Module['memory_arena'] = memory_arena;

  memory_arena.prototype['get_base'] = memory_arena.prototype.get_base = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_memory_arena_get_base_0(self), VoidPtr);
};
    memory_arena.prototype['set_base'] = memory_arena.prototype.set_base = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_memory_arena_set_base_1(self, arg0);
};
    Object.defineProperty(memory_arena.prototype, 'base', { get: memory_arena.prototype.get_base, set: memory_arena.prototype.set_base }) 
  memory_arena.prototype['get_size'] = memory_arena.prototype.get_size = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_memory_arena_get_size_0(self);
};
    memory_arena.prototype['set_size'] = memory_arena.prototype.set_size = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_memory_arena_set_size_1(self, arg0);
};
    Object.defineProperty(memory_arena.prototype, 'size', { get: memory_arena.prototype.get_size, set: memory_arena.prototype.set_size }) 
  memory_arena.prototype['get_capacity'] = memory_arena.prototype.get_capacity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_memory_arena_get_capacity_0(self);
};
    memory_arena.prototype['set_capacity'] = memory_arena.prototype.set_capacity = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_memory_arena_set_capacity_1(self, arg0);
};
    Object.defineProperty(memory_arena.prototype, 'capacity', { get: memory_arena.prototype.get_capacity, set: memory_arena.prototype.set_capacity }) 
  memory_arena.prototype['__destroy__'] = memory_arena.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_memory_arena___destroy___0(self);
};
// render_command_set_camera
/** @suppress {undefinedVars, duplicate} */function render_command_set_camera() { throw "cannot construct a render_command_set_camera, no constructor in IDL" }
render_command_set_camera.prototype = Object.create(WrapperObject.prototype);
render_command_set_camera.prototype.constructor = render_command_set_camera;
render_command_set_camera.prototype.__class__ = render_command_set_camera;
render_command_set_camera.__cache__ = {};
Module['render_command_set_camera'] = render_command_set_camera;

  render_command_set_camera.prototype['get_viewMatrix'] = render_command_set_camera.prototype.get_viewMatrix = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_render_command_set_camera_get_viewMatrix_0(self), matrix4x4);
};
    render_command_set_camera.prototype['set_viewMatrix'] = render_command_set_camera.prototype.set_viewMatrix = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_set_camera_set_viewMatrix_1(self, arg0);
};
    Object.defineProperty(render_command_set_camera.prototype, 'viewMatrix', { get: render_command_set_camera.prototype.get_viewMatrix, set: render_command_set_camera.prototype.set_viewMatrix }) 
  render_command_set_camera.prototype['get_projMatrix'] = render_command_set_camera.prototype.get_projMatrix = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_render_command_set_camera_get_projMatrix_0(self), matrix4x4);
};
    render_command_set_camera.prototype['set_projMatrix'] = render_command_set_camera.prototype.set_projMatrix = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_set_camera_set_projMatrix_1(self, arg0);
};
    Object.defineProperty(render_command_set_camera.prototype, 'projMatrix', { get: render_command_set_camera.prototype.get_projMatrix, set: render_command_set_camera.prototype.set_projMatrix }) 
  render_command_set_camera.prototype['__destroy__'] = render_command_set_camera.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_command_set_camera___destroy___0(self);
};
// render_command_header
/** @suppress {undefinedVars, duplicate} */function render_command_header() { throw "cannot construct a render_command_header, no constructor in IDL" }
render_command_header.prototype = Object.create(WrapperObject.prototype);
render_command_header.prototype.constructor = render_command_header;
render_command_header.prototype.__class__ = render_command_header;
render_command_header.__cache__ = {};
Module['render_command_header'] = render_command_header;

  render_command_header.prototype['get_type'] = render_command_header.prototype.get_type = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_command_header_get_type_0(self);
};
    render_command_header.prototype['set_type'] = render_command_header.prototype.set_type = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_header_set_type_1(self, arg0);
};
    Object.defineProperty(render_command_header.prototype, 'type', { get: render_command_header.prototype.get_type, set: render_command_header.prototype.set_type }) 
  render_command_header.prototype['__destroy__'] = render_command_header.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_command_header___destroy___0(self);
};
// aabb
/** @suppress {undefinedVars, duplicate} */function aabb() { throw "cannot construct a aabb, no constructor in IDL" }
aabb.prototype = Object.create(WrapperObject.prototype);
aabb.prototype.constructor = aabb;
aabb.prototype.__class__ = aabb;
aabb.__cache__ = {};
Module['aabb'] = aabb;

  aabb.prototype['get_min'] = aabb.prototype.get_min = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_aabb_get_min_0(self), vector3);
};
    aabb.prototype['set_min'] = aabb.prototype.set_min = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_aabb_set_min_1(self, arg0);
};
    Object.defineProperty(aabb.prototype, 'min', { get: aabb.prototype.get_min, set: aabb.prototype.set_min }) 
  aabb.prototype['get_max'] = aabb.prototype.get_max = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_aabb_get_max_0(self), vector3);
};
    aabb.prototype['set_max'] = aabb.prototype.set_max = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_aabb_set_max_1(self, arg0);
};
    Object.defineProperty(aabb.prototype, 'max', { get: aabb.prototype.get_max, set: aabb.prototype.set_max }) 
  aabb.prototype['__destroy__'] = aabb.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_aabb___destroy___0(self);
};
// vector3
/** @suppress {undefinedVars, duplicate} */function vector3() { throw "cannot construct a vector3, no constructor in IDL" }
vector3.prototype = Object.create(WrapperObject.prototype);
vector3.prototype.constructor = vector3;
vector3.prototype.__class__ = vector3;
vector3.__cache__ = {};
Module['vector3'] = vector3;

  vector3.prototype['get_x'] = vector3.prototype.get_x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_x_0(self);
};
    vector3.prototype['set_x'] = vector3.prototype.set_x = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_x_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'x', { get: vector3.prototype.get_x, set: vector3.prototype.set_x }) 
  vector3.prototype['get_y'] = vector3.prototype.get_y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_y_0(self);
};
    vector3.prototype['set_y'] = vector3.prototype.set_y = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_y_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'y', { get: vector3.prototype.get_y, set: vector3.prototype.set_y }) 
  vector3.prototype['get_z'] = vector3.prototype.get_z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_z_0(self);
};
    vector3.prototype['set_z'] = vector3.prototype.set_z = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_z_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'z', { get: vector3.prototype.get_z, set: vector3.prototype.set_z }) 
  vector3.prototype['get_r'] = vector3.prototype.get_r = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_r_0(self);
};
    vector3.prototype['set_r'] = vector3.prototype.set_r = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_r_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'r', { get: vector3.prototype.get_r, set: vector3.prototype.set_r }) 
  vector3.prototype['get_g'] = vector3.prototype.get_g = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_g_0(self);
};
    vector3.prototype['set_g'] = vector3.prototype.set_g = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_g_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'g', { get: vector3.prototype.get_g, set: vector3.prototype.set_g }) 
  vector3.prototype['get_b'] = vector3.prototype.get_b = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_vector3_get_b_0(self);
};
    vector3.prototype['set_b'] = vector3.prototype.set_b = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_vector3_set_b_1(self, arg0);
};
    Object.defineProperty(vector3.prototype, 'b', { get: vector3.prototype.get_b, set: vector3.prototype.set_b }) 
  vector3.prototype['get_c'] = vector3.prototype.get_c = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return _emscripten_bind_vector3_get_c_1(self, arg0);
};
    vector3.prototype['set_c'] = vector3.prototype.set_c = /** @suppress {undefinedVars, duplicate} */function(arg0, arg1) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_vector3_set_c_2(self, arg0, arg1);
};
    Object.defineProperty(vector3.prototype, 'c', { get: vector3.prototype.get_c, set: vector3.prototype.set_c }) 
  vector3.prototype['__destroy__'] = vector3.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_vector3___destroy___0(self);
};
// render_command_model
/** @suppress {undefinedVars, duplicate} */function render_command_model() { throw "cannot construct a render_command_model, no constructor in IDL" }
render_command_model.prototype = Object.create(WrapperObject.prototype);
render_command_model.prototype.constructor = render_command_model;
render_command_model.prototype.__class__ = render_command_model;
render_command_model.__cache__ = {};
Module['render_command_model'] = render_command_model;

  render_command_model.prototype['get_meshKey'] = render_command_model.prototype.get_meshKey = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_command_model_get_meshKey_0(self);
};
    render_command_model.prototype['set_meshKey'] = render_command_model.prototype.set_meshKey = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_model_set_meshKey_1(self, arg0);
};
    Object.defineProperty(render_command_model.prototype, 'meshKey', { get: render_command_model.prototype.get_meshKey, set: render_command_model.prototype.set_meshKey }) 
  render_command_model.prototype['get_textureKey'] = render_command_model.prototype.get_textureKey = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_command_model_get_textureKey_0(self);
};
    render_command_model.prototype['set_textureKey'] = render_command_model.prototype.set_textureKey = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_model_set_textureKey_1(self, arg0);
};
    Object.defineProperty(render_command_model.prototype, 'textureKey', { get: render_command_model.prototype.get_textureKey, set: render_command_model.prototype.set_textureKey }) 
  render_command_model.prototype['get_modelMatrix'] = render_command_model.prototype.get_modelMatrix = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_render_command_model_get_modelMatrix_0(self), matrix4x4);
};
    render_command_model.prototype['set_modelMatrix'] = render_command_model.prototype.set_modelMatrix = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_model_set_modelMatrix_1(self, arg0);
};
    Object.defineProperty(render_command_model.prototype, 'modelMatrix', { get: render_command_model.prototype.get_modelMatrix, set: render_command_model.prototype.set_modelMatrix }) 
  render_command_model.prototype['__destroy__'] = render_command_model.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_command_model___destroy___0(self);
};
// game_input
/** @suppress {undefinedVars, duplicate} */function game_input() { throw "cannot construct a game_input, no constructor in IDL" }
game_input.prototype = Object.create(WrapperObject.prototype);
game_input.prototype.constructor = game_input;
game_input.prototype.__class__ = game_input;
game_input.__cache__ = {};
Module['game_input'] = game_input;

  game_input.prototype['get_forwardButton'] = game_input.prototype.get_forwardButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_forwardButton_0(self));
};
    game_input.prototype['set_forwardButton'] = game_input.prototype.set_forwardButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_forwardButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'forwardButton', { get: game_input.prototype.get_forwardButton, set: game_input.prototype.set_forwardButton }) 
  game_input.prototype['get_backButton'] = game_input.prototype.get_backButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_backButton_0(self));
};
    game_input.prototype['set_backButton'] = game_input.prototype.set_backButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_backButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'backButton', { get: game_input.prototype.get_backButton, set: game_input.prototype.set_backButton }) 
  game_input.prototype['get_leftButton'] = game_input.prototype.get_leftButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_leftButton_0(self));
};
    game_input.prototype['set_leftButton'] = game_input.prototype.set_leftButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_leftButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'leftButton', { get: game_input.prototype.get_leftButton, set: game_input.prototype.set_leftButton }) 
  game_input.prototype['get_rightButton'] = game_input.prototype.get_rightButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_rightButton_0(self));
};
    game_input.prototype['set_rightButton'] = game_input.prototype.set_rightButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_rightButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'rightButton', { get: game_input.prototype.get_rightButton, set: game_input.prototype.set_rightButton }) 
  game_input.prototype['get_upButton'] = game_input.prototype.get_upButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_upButton_0(self));
};
    game_input.prototype['set_upButton'] = game_input.prototype.set_upButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_upButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'upButton', { get: game_input.prototype.get_upButton, set: game_input.prototype.set_upButton }) 
  game_input.prototype['get_downButton'] = game_input.prototype.get_downButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_downButton_0(self));
};
    game_input.prototype['set_downButton'] = game_input.prototype.set_downButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_downButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'downButton', { get: game_input.prototype.get_downButton, set: game_input.prototype.set_downButton }) 
  game_input.prototype['get_turnUpButton'] = game_input.prototype.get_turnUpButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_turnUpButton_0(self));
};
    game_input.prototype['set_turnUpButton'] = game_input.prototype.set_turnUpButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_turnUpButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'turnUpButton', { get: game_input.prototype.get_turnUpButton, set: game_input.prototype.set_turnUpButton }) 
  game_input.prototype['get_turnDownButton'] = game_input.prototype.get_turnDownButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_turnDownButton_0(self));
};
    game_input.prototype['set_turnDownButton'] = game_input.prototype.set_turnDownButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_turnDownButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'turnDownButton', { get: game_input.prototype.get_turnDownButton, set: game_input.prototype.set_turnDownButton }) 
  game_input.prototype['get_turnLeftButton'] = game_input.prototype.get_turnLeftButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_turnLeftButton_0(self));
};
    game_input.prototype['set_turnLeftButton'] = game_input.prototype.set_turnLeftButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_turnLeftButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'turnLeftButton', { get: game_input.prototype.get_turnLeftButton, set: game_input.prototype.set_turnLeftButton }) 
  game_input.prototype['get_turnRightButton'] = game_input.prototype.get_turnRightButton = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_turnRightButton_0(self));
};
    game_input.prototype['set_turnRightButton'] = game_input.prototype.set_turnRightButton = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_turnRightButton_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'turnRightButton', { get: game_input.prototype.get_turnRightButton, set: game_input.prototype.set_turnRightButton }) 
  game_input.prototype['get_pointerDown'] = game_input.prototype.get_pointerDown = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_pointerDown_0(self));
};
    game_input.prototype['set_pointerDown'] = game_input.prototype.set_pointerDown = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_pointerDown_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'pointerDown', { get: game_input.prototype.get_pointerDown, set: game_input.prototype.set_pointerDown }) 
  game_input.prototype['get_pointerJustDown'] = game_input.prototype.get_pointerJustDown = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_game_input_get_pointerJustDown_0(self));
};
    game_input.prototype['set_pointerJustDown'] = game_input.prototype.set_pointerJustDown = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_pointerJustDown_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'pointerJustDown', { get: game_input.prototype.get_pointerJustDown, set: game_input.prototype.set_pointerJustDown }) 
  game_input.prototype['get_pointerX'] = game_input.prototype.get_pointerX = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_game_input_get_pointerX_0(self);
};
    game_input.prototype['set_pointerX'] = game_input.prototype.set_pointerX = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_pointerX_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'pointerX', { get: game_input.prototype.get_pointerX, set: game_input.prototype.set_pointerX }) 
  game_input.prototype['get_pointerY'] = game_input.prototype.get_pointerY = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_game_input_get_pointerY_0(self);
};
    game_input.prototype['set_pointerY'] = game_input.prototype.set_pointerY = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_input_set_pointerY_1(self, arg0);
};
    Object.defineProperty(game_input.prototype, 'pointerY', { get: game_input.prototype.get_pointerY, set: game_input.prototype.set_pointerY }) 
  game_input.prototype['__destroy__'] = game_input.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_game_input___destroy___0(self);
};
// matrix4x4
/** @suppress {undefinedVars, duplicate} */function matrix4x4() { throw "cannot construct a matrix4x4, no constructor in IDL" }
matrix4x4.prototype = Object.create(WrapperObject.prototype);
matrix4x4.prototype.constructor = matrix4x4;
matrix4x4.prototype.__class__ = matrix4x4;
matrix4x4.__cache__ = {};
Module['matrix4x4'] = matrix4x4;

  matrix4x4.prototype['get_m'] = matrix4x4.prototype.get_m = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return _emscripten_bind_matrix4x4_get_m_1(self, arg0);
};
    matrix4x4.prototype['set_m'] = matrix4x4.prototype.set_m = /** @suppress {undefinedVars, duplicate} */function(arg0, arg1) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_matrix4x4_set_m_2(self, arg0, arg1);
};
    Object.defineProperty(matrix4x4.prototype, 'm', { get: matrix4x4.prototype.get_m, set: matrix4x4.prototype.set_m }) 
  matrix4x4.prototype['__destroy__'] = matrix4x4.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_matrix4x4___destroy___0(self);
};
// sphere
/** @suppress {undefinedVars, duplicate} */function sphere() { throw "cannot construct a sphere, no constructor in IDL" }
sphere.prototype = Object.create(WrapperObject.prototype);
sphere.prototype.constructor = sphere;
sphere.prototype.__class__ = sphere;
sphere.__cache__ = {};
Module['sphere'] = sphere;

  sphere.prototype['get_pos'] = sphere.prototype.get_pos = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_sphere_get_pos_0(self), vector3);
};
    sphere.prototype['set_pos'] = sphere.prototype.set_pos = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_sphere_set_pos_1(self, arg0);
};
    Object.defineProperty(sphere.prototype, 'pos', { get: sphere.prototype.get_pos, set: sphere.prototype.set_pos }) 
  sphere.prototype['get_radius'] = sphere.prototype.get_radius = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_sphere_get_radius_0(self);
};
    sphere.prototype['set_radius'] = sphere.prototype.set_radius = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_sphere_set_radius_1(self, arg0);
};
    Object.defineProperty(sphere.prototype, 'radius', { get: sphere.prototype.get_radius, set: sphere.prototype.set_radius }) 
  sphere.prototype['__destroy__'] = sphere.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_sphere___destroy___0(self);
};
// quaternion
/** @suppress {undefinedVars, duplicate} */function quaternion() { throw "cannot construct a quaternion, no constructor in IDL" }
quaternion.prototype = Object.create(WrapperObject.prototype);
quaternion.prototype.constructor = quaternion;
quaternion.prototype.__class__ = quaternion;
quaternion.__cache__ = {};
Module['quaternion'] = quaternion;

  quaternion.prototype['get_w'] = quaternion.prototype.get_w = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_quaternion_get_w_0(self);
};
    quaternion.prototype['set_w'] = quaternion.prototype.set_w = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_quaternion_set_w_1(self, arg0);
};
    Object.defineProperty(quaternion.prototype, 'w', { get: quaternion.prototype.get_w, set: quaternion.prototype.set_w }) 
  quaternion.prototype['get_x'] = quaternion.prototype.get_x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_quaternion_get_x_0(self);
};
    quaternion.prototype['set_x'] = quaternion.prototype.set_x = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_quaternion_set_x_1(self, arg0);
};
    Object.defineProperty(quaternion.prototype, 'x', { get: quaternion.prototype.get_x, set: quaternion.prototype.set_x }) 
  quaternion.prototype['get_y'] = quaternion.prototype.get_y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_quaternion_get_y_0(self);
};
    quaternion.prototype['set_y'] = quaternion.prototype.set_y = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_quaternion_set_y_1(self, arg0);
};
    Object.defineProperty(quaternion.prototype, 'y', { get: quaternion.prototype.get_y, set: quaternion.prototype.set_y }) 
  quaternion.prototype['get_z'] = quaternion.prototype.get_z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_quaternion_get_z_0(self);
};
    quaternion.prototype['set_z'] = quaternion.prototype.set_z = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_quaternion_set_z_1(self, arg0);
};
    Object.defineProperty(quaternion.prototype, 'z', { get: quaternion.prototype.get_z, set: quaternion.prototype.set_z }) 
  quaternion.prototype['__destroy__'] = quaternion.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_quaternion___destroy___0(self);
};
// plane
/** @suppress {undefinedVars, duplicate} */function plane() { throw "cannot construct a plane, no constructor in IDL" }
plane.prototype = Object.create(WrapperObject.prototype);
plane.prototype.constructor = plane;
plane.prototype.__class__ = plane;
plane.__cache__ = {};
Module['plane'] = plane;

  plane.prototype['get_normal'] = plane.prototype.get_normal = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_plane_get_normal_0(self), vector3);
};
    plane.prototype['set_normal'] = plane.prototype.set_normal = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_plane_set_normal_1(self, arg0);
};
    Object.defineProperty(plane.prototype, 'normal', { get: plane.prototype.get_normal, set: plane.prototype.set_normal }) 
  plane.prototype['get_distance'] = plane.prototype.get_distance = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_plane_get_distance_0(self);
};
    plane.prototype['set_distance'] = plane.prototype.set_distance = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_plane_set_distance_1(self, arg0);
};
    Object.defineProperty(plane.prototype, 'distance', { get: plane.prototype.get_distance, set: plane.prototype.set_distance }) 
  plane.prototype['__destroy__'] = plane.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_plane___destroy___0(self);
};
// render_command_list
/** @suppress {undefinedVars, duplicate} */function render_command_list() { throw "cannot construct a render_command_list, no constructor in IDL" }
render_command_list.prototype = Object.create(WrapperObject.prototype);
render_command_list.prototype.constructor = render_command_list;
render_command_list.prototype.__class__ = render_command_list;
render_command_list.__cache__ = {};
Module['render_command_list'] = render_command_list;

  render_command_list.prototype['get_memory'] = render_command_list.prototype.get_memory = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_render_command_list_get_memory_0(self), memory_arena);
};
    render_command_list.prototype['set_memory'] = render_command_list.prototype.set_memory = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_command_list_set_memory_1(self, arg0);
};
    Object.defineProperty(render_command_list.prototype, 'memory', { get: render_command_list.prototype.get_memory, set: render_command_list.prototype.set_memory }) 
  render_command_list.prototype['__destroy__'] = render_command_list.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_command_list___destroy___0(self);
};
// loaded_texture_asset
/** @suppress {undefinedVars, duplicate} */function loaded_texture_asset() { throw "cannot construct a loaded_texture_asset, no constructor in IDL" }
loaded_texture_asset.prototype = Object.create(WrapperObject.prototype);
loaded_texture_asset.prototype.constructor = loaded_texture_asset;
loaded_texture_asset.prototype.__class__ = loaded_texture_asset;
loaded_texture_asset.__cache__ = {};
Module['loaded_texture_asset'] = loaded_texture_asset;

  loaded_texture_asset.prototype['get_key'] = loaded_texture_asset.prototype.get_key = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_loaded_texture_asset_get_key_0(self);
};
    loaded_texture_asset.prototype['set_key'] = loaded_texture_asset.prototype.set_key = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_texture_asset_set_key_1(self, arg0);
};
    Object.defineProperty(loaded_texture_asset.prototype, 'key', { get: loaded_texture_asset.prototype.get_key, set: loaded_texture_asset.prototype.set_key }) 
  loaded_texture_asset.prototype['get_width'] = loaded_texture_asset.prototype.get_width = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_loaded_texture_asset_get_width_0(self);
};
    loaded_texture_asset.prototype['set_width'] = loaded_texture_asset.prototype.set_width = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_texture_asset_set_width_1(self, arg0);
};
    Object.defineProperty(loaded_texture_asset.prototype, 'width', { get: loaded_texture_asset.prototype.get_width, set: loaded_texture_asset.prototype.set_width }) 
  loaded_texture_asset.prototype['get_height'] = loaded_texture_asset.prototype.get_height = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_loaded_texture_asset_get_height_0(self);
};
    loaded_texture_asset.prototype['set_height'] = loaded_texture_asset.prototype.set_height = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_texture_asset_set_height_1(self, arg0);
};
    Object.defineProperty(loaded_texture_asset.prototype, 'height', { get: loaded_texture_asset.prototype.get_height, set: loaded_texture_asset.prototype.set_height }) 
  loaded_texture_asset.prototype['get_pixels'] = loaded_texture_asset.prototype.get_pixels = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_loaded_texture_asset_get_pixels_0(self), VoidPtr);
};
    loaded_texture_asset.prototype['set_pixels'] = loaded_texture_asset.prototype.set_pixels = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_loaded_texture_asset_set_pixels_1(self, arg0);
};
    Object.defineProperty(loaded_texture_asset.prototype, 'pixels', { get: loaded_texture_asset.prototype.get_pixels, set: loaded_texture_asset.prototype.set_pixels }) 
  loaded_texture_asset.prototype['__destroy__'] = loaded_texture_asset.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_loaded_texture_asset___destroy___0(self);
};
// matrix3x3
/** @suppress {undefinedVars, duplicate} */function matrix3x3() { throw "cannot construct a matrix3x3, no constructor in IDL" }
matrix3x3.prototype = Object.create(WrapperObject.prototype);
matrix3x3.prototype.constructor = matrix3x3;
matrix3x3.prototype.__class__ = matrix3x3;
matrix3x3.__cache__ = {};
Module['matrix3x3'] = matrix3x3;

  matrix3x3.prototype['get_m'] = matrix3x3.prototype.get_m = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return _emscripten_bind_matrix3x3_get_m_1(self, arg0);
};
    matrix3x3.prototype['set_m'] = matrix3x3.prototype.set_m = /** @suppress {undefinedVars, duplicate} */function(arg0, arg1) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_matrix3x3_set_m_2(self, arg0, arg1);
};
    Object.defineProperty(matrix3x3.prototype, 'm', { get: matrix3x3.prototype.get_m, set: matrix3x3.prototype.set_m }) 
  matrix3x3.prototype['__destroy__'] = matrix3x3.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_matrix3x3___destroy___0(self);
};
// asset_list
/** @suppress {undefinedVars, duplicate} */function asset_list() { throw "cannot construct a asset_list, no constructor in IDL" }
asset_list.prototype = Object.create(WrapperObject.prototype);
asset_list.prototype.constructor = asset_list;
asset_list.prototype.__class__ = asset_list;
asset_list.__cache__ = {};
Module['asset_list'] = asset_list;

  asset_list.prototype['get_numAssetsToLoad'] = asset_list.prototype.get_numAssetsToLoad = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_list_get_numAssetsToLoad_0(self);
};
    asset_list.prototype['set_numAssetsToLoad'] = asset_list.prototype.set_numAssetsToLoad = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_list_set_numAssetsToLoad_1(self, arg0);
};
    Object.defineProperty(asset_list.prototype, 'numAssetsToLoad', { get: asset_list.prototype.get_numAssetsToLoad, set: asset_list.prototype.set_numAssetsToLoad }) 
  asset_list.prototype['get_maxAssetsToLoad'] = asset_list.prototype.get_maxAssetsToLoad = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_list_get_maxAssetsToLoad_0(self);
};
    asset_list.prototype['set_maxAssetsToLoad'] = asset_list.prototype.set_maxAssetsToLoad = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_list_set_maxAssetsToLoad_1(self, arg0);
};
    Object.defineProperty(asset_list.prototype, 'maxAssetsToLoad', { get: asset_list.prototype.get_maxAssetsToLoad, set: asset_list.prototype.set_maxAssetsToLoad }) 
  asset_list.prototype['get_assetsToLoad'] = asset_list.prototype.get_assetsToLoad = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_asset_list_get_assetsToLoad_0(self), asset_to_load);
};
    asset_list.prototype['set_assetsToLoad'] = asset_list.prototype.set_assetsToLoad = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_list_set_assetsToLoad_1(self, arg0);
};
    Object.defineProperty(asset_list.prototype, 'assetsToLoad', { get: asset_list.prototype.get_assetsToLoad, set: asset_list.prototype.set_assetsToLoad }) 
  asset_list.prototype['__destroy__'] = asset_list.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_asset_list___destroy___0(self);
};
// asset_to_load
/** @suppress {undefinedVars, duplicate} */function asset_to_load() { throw "cannot construct a asset_to_load, no constructor in IDL" }
asset_to_load.prototype = Object.create(WrapperObject.prototype);
asset_to_load.prototype.constructor = asset_to_load;
asset_to_load.prototype.__class__ = asset_to_load;
asset_to_load.__cache__ = {};
Module['asset_to_load'] = asset_to_load;

  asset_to_load.prototype['get_path'] = asset_to_load.prototype.get_path = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_asset_to_load_get_path_0(self));
};
    asset_to_load.prototype['set_path'] = asset_to_load.prototype.set_path = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_asset_to_load_set_path_1(self, arg0);
};
    Object.defineProperty(asset_to_load.prototype, 'path', { get: asset_to_load.prototype.get_path, set: asset_to_load.prototype.set_path }) 
  asset_to_load.prototype['get_type'] = asset_to_load.prototype.get_type = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_to_load_get_type_0(self);
};
    asset_to_load.prototype['set_type'] = asset_to_load.prototype.set_type = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_to_load_set_type_1(self, arg0);
};
    Object.defineProperty(asset_to_load.prototype, 'type', { get: asset_to_load.prototype.get_type, set: asset_to_load.prototype.set_type }) 
  asset_to_load.prototype['get_key'] = asset_to_load.prototype.get_key = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_to_load_get_key_0(self);
};
    asset_to_load.prototype['set_key'] = asset_to_load.prototype.set_key = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_to_load_set_key_1(self, arg0);
};
    Object.defineProperty(asset_to_load.prototype, 'key', { get: asset_to_load.prototype.get_key, set: asset_to_load.prototype.set_key }) 
  asset_to_load.prototype['__destroy__'] = asset_to_load.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_asset_to_load___destroy___0(self);
};
// VoidPtr
/** @suppress {undefinedVars, duplicate} */function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// game_memory
/** @suppress {undefinedVars, duplicate} */function game_memory() { throw "cannot construct a game_memory, no constructor in IDL" }
game_memory.prototype = Object.create(WrapperObject.prototype);
game_memory.prototype.constructor = game_memory;
game_memory.prototype.__class__ = game_memory;
game_memory.__cache__ = {};
Module['game_memory'] = game_memory;

  game_memory.prototype['get_memory'] = game_memory.prototype.get_memory = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_game_memory_get_memory_0(self), VoidPtr);
};
    game_memory.prototype['set_memory'] = game_memory.prototype.set_memory = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_memory_set_memory_1(self, arg0);
};
    Object.defineProperty(game_memory.prototype, 'memory', { get: game_memory.prototype.get_memory, set: game_memory.prototype.set_memory }) 
  game_memory.prototype['get_memoryCapacity'] = game_memory.prototype.get_memoryCapacity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_game_memory_get_memoryCapacity_0(self);
};
    game_memory.prototype['set_memoryCapacity'] = game_memory.prototype.set_memoryCapacity = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_game_memory_set_memoryCapacity_1(self, arg0);
};
    Object.defineProperty(game_memory.prototype, 'memoryCapacity', { get: game_memory.prototype.get_memoryCapacity, set: game_memory.prototype.set_memoryCapacity }) 
  game_memory.prototype['__destroy__'] = game_memory.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_game_memory___destroy___0(self);
};
// int_mesh_attribute
/** @suppress {undefinedVars, duplicate} */function int_mesh_attribute() { throw "cannot construct a int_mesh_attribute, no constructor in IDL" }
int_mesh_attribute.prototype = Object.create(WrapperObject.prototype);
int_mesh_attribute.prototype.constructor = int_mesh_attribute;
int_mesh_attribute.prototype.__class__ = int_mesh_attribute;
int_mesh_attribute.__cache__ = {};
Module['int_mesh_attribute'] = int_mesh_attribute;

  int_mesh_attribute.prototype['get_values'] = int_mesh_attribute.prototype.get_values = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_int_mesh_attribute_get_values_0(self), VoidPtr);
};
    int_mesh_attribute.prototype['set_values'] = int_mesh_attribute.prototype.set_values = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_int_mesh_attribute_set_values_1(self, arg0);
};
    Object.defineProperty(int_mesh_attribute.prototype, 'values', { get: int_mesh_attribute.prototype.get_values, set: int_mesh_attribute.prototype.set_values }) 
  int_mesh_attribute.prototype['get_count'] = int_mesh_attribute.prototype.get_count = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_int_mesh_attribute_get_count_0(self);
};
    int_mesh_attribute.prototype['set_count'] = int_mesh_attribute.prototype.set_count = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_int_mesh_attribute_set_count_1(self, arg0);
};
    Object.defineProperty(int_mesh_attribute.prototype, 'count', { get: int_mesh_attribute.prototype.get_count, set: int_mesh_attribute.prototype.set_count }) 
  int_mesh_attribute.prototype['__destroy__'] = int_mesh_attribute.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_int_mesh_attribute___destroy___0(self);
};
(function() {
  function setupEnums() {
    

    // asset_type

    Module['ASSET_TYPE_OBJ'] = _emscripten_enum_asset_type_ASSET_TYPE_OBJ();

    Module['ASSET_TYPE_BMP'] = _emscripten_enum_asset_type_ASSET_TYPE_BMP();

    

    // render_command_type

    Module['RENDER_COMMAND_MODEL'] = _emscripten_enum_render_command_type_RENDER_COMMAND_MODEL();

    Module['RENDER_COMMAND_SET_CAMERA'] = _emscripten_enum_render_command_type_RENDER_COMMAND_SET_CAMERA();

  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
