
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


// render_rectangle_command
/** @suppress {undefinedVars, duplicate} */function render_rectangle_command() { throw "cannot construct a render_rectangle_command, no constructor in IDL" }
render_rectangle_command.prototype = Object.create(WrapperObject.prototype);
render_rectangle_command.prototype.constructor = render_rectangle_command;
render_rectangle_command.prototype.__class__ = render_rectangle_command;
render_rectangle_command.__cache__ = {};
Module['render_rectangle_command'] = render_rectangle_command;

  render_rectangle_command.prototype['get_x'] = render_rectangle_command.prototype.get_x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_rectangle_command_get_x_0(self);
};
    render_rectangle_command.prototype['set_x'] = render_rectangle_command.prototype.set_x = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_rectangle_command_set_x_1(self, arg0);
};
    Object.defineProperty(render_rectangle_command.prototype, 'x', { get: render_rectangle_command.prototype.get_x, set: render_rectangle_command.prototype.set_x }) 
  render_rectangle_command.prototype['get_y'] = render_rectangle_command.prototype.get_y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_rectangle_command_get_y_0(self);
};
    render_rectangle_command.prototype['set_y'] = render_rectangle_command.prototype.set_y = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_rectangle_command_set_y_1(self, arg0);
};
    Object.defineProperty(render_rectangle_command.prototype, 'y', { get: render_rectangle_command.prototype.get_y, set: render_rectangle_command.prototype.set_y }) 
  render_rectangle_command.prototype['get_width'] = render_rectangle_command.prototype.get_width = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_rectangle_command_get_width_0(self);
};
    render_rectangle_command.prototype['set_width'] = render_rectangle_command.prototype.set_width = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_rectangle_command_set_width_1(self, arg0);
};
    Object.defineProperty(render_rectangle_command.prototype, 'width', { get: render_rectangle_command.prototype.get_width, set: render_rectangle_command.prototype.set_width }) 
  render_rectangle_command.prototype['get_height'] = render_rectangle_command.prototype.get_height = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_rectangle_command_get_height_0(self);
};
    render_rectangle_command.prototype['set_height'] = render_rectangle_command.prototype.set_height = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_rectangle_command_set_height_1(self, arg0);
};
    Object.defineProperty(render_rectangle_command.prototype, 'height', { get: render_rectangle_command.prototype.get_height, set: render_rectangle_command.prototype.set_height }) 
  render_rectangle_command.prototype['get_color'] = render_rectangle_command.prototype.get_color = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_rectangle_command_get_color_0(self);
};
    render_rectangle_command.prototype['set_color'] = render_rectangle_command.prototype.set_color = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_rectangle_command_set_color_1(self, arg0);
};
    Object.defineProperty(render_rectangle_command.prototype, 'color', { get: render_rectangle_command.prototype.get_color, set: render_rectangle_command.prototype.set_color }) 
  render_rectangle_command.prototype['__destroy__'] = render_rectangle_command.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_rectangle_command___destroy___0(self);
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
// render_horizontal_line_command
/** @suppress {undefinedVars, duplicate} */function render_horizontal_line_command() { throw "cannot construct a render_horizontal_line_command, no constructor in IDL" }
render_horizontal_line_command.prototype = Object.create(WrapperObject.prototype);
render_horizontal_line_command.prototype.constructor = render_horizontal_line_command;
render_horizontal_line_command.prototype.__class__ = render_horizontal_line_command;
render_horizontal_line_command.__cache__ = {};
Module['render_horizontal_line_command'] = render_horizontal_line_command;

  render_horizontal_line_command.prototype['get_color'] = render_horizontal_line_command.prototype.get_color = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_horizontal_line_command_get_color_0(self);
};
    render_horizontal_line_command.prototype['set_color'] = render_horizontal_line_command.prototype.set_color = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_horizontal_line_command_set_color_1(self, arg0);
};
    Object.defineProperty(render_horizontal_line_command.prototype, 'color', { get: render_horizontal_line_command.prototype.get_color, set: render_horizontal_line_command.prototype.set_color }) 
  render_horizontal_line_command.prototype['get_lineNum'] = render_horizontal_line_command.prototype.get_lineNum = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_render_horizontal_line_command_get_lineNum_0(self);
};
    render_horizontal_line_command.prototype['set_lineNum'] = render_horizontal_line_command.prototype.set_lineNum = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_render_horizontal_line_command_set_lineNum_1(self, arg0);
};
    Object.defineProperty(render_horizontal_line_command.prototype, 'lineNum', { get: render_horizontal_line_command.prototype.get_lineNum, set: render_horizontal_line_command.prototype.set_lineNum }) 
  render_horizontal_line_command.prototype['__destroy__'] = render_horizontal_line_command.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_render_horizontal_line_command___destroy___0(self);
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
(function() {
  function setupEnums() {
    

    // render_command_type

    Module['RENDER_COMMAND_RECTANGLE'] = _emscripten_enum_render_command_type_RENDER_COMMAND_RECTANGLE();

    Module['RENDER_COMMAND_HORIZONTAL_LINE'] = _emscripten_enum_render_command_type_RENDER_COMMAND_HORIZONTAL_LINE();

  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
