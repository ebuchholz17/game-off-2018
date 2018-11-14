
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
// render_mesh_command
/** @suppress {undefinedVars, duplicate} */function render_mesh_command() { throw "cannot construct a render_mesh_command, no constructor in IDL" }
render_mesh_command.prototype = Object.create(WrapperObject.prototype);
render_mesh_command.prototype.constructor = render_mesh_command;
render_mesh_command.prototype.__class__ = render_mesh_command;
render_mesh_command.__cache__ = {};
Module['render_mesh_command'] = render_mesh_command;

  render_mesh_command.prototype['get_key'] = render_mesh_command.prototype.get_key = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_mesh_command_get_key_0(self);
};
    render_mesh_command.prototype['set_key'] = render_mesh_command.prototype.set_key = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_mesh_command_set_key_1(self, arg0);
};
    Object.defineProperty(render_mesh_command.prototype, 'key', { get: render_mesh_command.prototype.get_key, set: render_mesh_command.prototype.set_key }) 
  render_mesh_command.prototype['__destroy__'] = render_mesh_command.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_mesh_command___destroy___0(self);
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

    

    // render_command_type

    Module['RENDER_COMMAND_MESH'] = _emscripten_enum_render_command_type_RENDER_COMMAND_MESH();

  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
