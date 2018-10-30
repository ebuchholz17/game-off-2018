
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


// rectangle_list
/** @suppress {undefinedVars, duplicate} */function rectangle_list() { throw "cannot construct a rectangle_list, no constructor in IDL" }
rectangle_list.prototype = Object.create(WrapperObject.prototype);
rectangle_list.prototype.constructor = rectangle_list;
rectangle_list.prototype.__class__ = rectangle_list;
rectangle_list.__cache__ = {};
Module['rectangle_list'] = rectangle_list;

  rectangle_list.prototype['get_numRectangles'] = rectangle_list.prototype.get_numRectangles = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_rectangle_list_get_numRectangles_0(self);
};
    rectangle_list.prototype['set_numRectangles'] = rectangle_list.prototype.set_numRectangles = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_rectangle_list_set_numRectangles_1(self, arg0);
};
    Object.defineProperty(rectangle_list.prototype, 'numRectangles', { get: rectangle_list.prototype.get_numRectangles, set: rectangle_list.prototype.set_numRectangles }) 
  rectangle_list.prototype['get_maxRectangles'] = rectangle_list.prototype.get_maxRectangles = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_rectangle_list_get_maxRectangles_0(self);
};
    rectangle_list.prototype['set_maxRectangles'] = rectangle_list.prototype.set_maxRectangles = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_rectangle_list_set_maxRectangles_1(self, arg0);
};
    Object.defineProperty(rectangle_list.prototype, 'maxRectangles', { get: rectangle_list.prototype.get_maxRectangles, set: rectangle_list.prototype.set_maxRectangles }) 
  rectangle_list.prototype['get_rectangles'] = rectangle_list.prototype.get_rectangles = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_rectangle_list_get_rectangles_0(self), colored_rectangle);
};
    rectangle_list.prototype['set_rectangles'] = rectangle_list.prototype.set_rectangles = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_rectangle_list_set_rectangles_1(self, arg0);
};
    Object.defineProperty(rectangle_list.prototype, 'rectangles', { get: rectangle_list.prototype.get_rectangles, set: rectangle_list.prototype.set_rectangles }) 
// colored_rectangle
/** @suppress {undefinedVars, duplicate} */function colored_rectangle() { throw "cannot construct a colored_rectangle, no constructor in IDL" }
colored_rectangle.prototype = Object.create(WrapperObject.prototype);
colored_rectangle.prototype.constructor = colored_rectangle;
colored_rectangle.prototype.__class__ = colored_rectangle;
colored_rectangle.__cache__ = {};
Module['colored_rectangle'] = colored_rectangle;

  colored_rectangle.prototype['get_x'] = colored_rectangle.prototype.get_x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_colored_rectangle_get_x_0(self);
};
    colored_rectangle.prototype['set_x'] = colored_rectangle.prototype.set_x = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_colored_rectangle_set_x_1(self, arg0);
};
    Object.defineProperty(colored_rectangle.prototype, 'x', { get: colored_rectangle.prototype.get_x, set: colored_rectangle.prototype.set_x }) 
  colored_rectangle.prototype['get_y'] = colored_rectangle.prototype.get_y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_colored_rectangle_get_y_0(self);
};
    colored_rectangle.prototype['set_y'] = colored_rectangle.prototype.set_y = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_colored_rectangle_set_y_1(self, arg0);
};
    Object.defineProperty(colored_rectangle.prototype, 'y', { get: colored_rectangle.prototype.get_y, set: colored_rectangle.prototype.set_y }) 
  colored_rectangle.prototype['get_width'] = colored_rectangle.prototype.get_width = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_colored_rectangle_get_width_0(self);
};
    colored_rectangle.prototype['set_width'] = colored_rectangle.prototype.set_width = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_colored_rectangle_set_width_1(self, arg0);
};
    Object.defineProperty(colored_rectangle.prototype, 'width', { get: colored_rectangle.prototype.get_width, set: colored_rectangle.prototype.set_width }) 
  colored_rectangle.prototype['get_height'] = colored_rectangle.prototype.get_height = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_colored_rectangle_get_height_0(self);
};
    colored_rectangle.prototype['set_height'] = colored_rectangle.prototype.set_height = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_colored_rectangle_set_height_1(self, arg0);
};
    Object.defineProperty(colored_rectangle.prototype, 'height', { get: colored_rectangle.prototype.get_height, set: colored_rectangle.prototype.set_height }) 
  colored_rectangle.prototype['get_color'] = colored_rectangle.prototype.get_color = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_colored_rectangle_get_color_0(self);
};
    colored_rectangle.prototype['set_color'] = colored_rectangle.prototype.set_color = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_colored_rectangle_set_color_1(self, arg0);
};
    Object.defineProperty(colored_rectangle.prototype, 'color', { get: colored_rectangle.prototype.get_color, set: colored_rectangle.prototype.set_color }) 
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
(function() {
  function setupEnums() {
    
  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
