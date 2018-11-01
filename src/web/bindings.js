
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
  rectangle_list.prototype['__destroy__'] = rectangle_list.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_rectangle_list___destroy___0(self);
};
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
  colored_rectangle.prototype['__destroy__'] = colored_rectangle.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_colored_rectangle___destroy___0(self);
};
// font_asset
/** @suppress {undefinedVars, duplicate} */function font_asset() { throw "cannot construct a font_asset, no constructor in IDL" }
font_asset.prototype = Object.create(WrapperObject.prototype);
font_asset.prototype.constructor = font_asset;
font_asset.prototype.__class__ = font_asset;
font_asset.__cache__ = {};
Module['font_asset'] = font_asset;

  font_asset.prototype['get_id'] = font_asset.prototype.get_id = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_font_asset_get_id_0(self);
};
    font_asset.prototype['set_id'] = font_asset.prototype.set_id = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_font_asset_set_id_1(self, arg0);
};
    Object.defineProperty(font_asset.prototype, 'id', { get: font_asset.prototype.get_id, set: font_asset.prototype.set_id }) 
  font_asset.prototype['get_name'] = font_asset.prototype.get_name = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_font_asset_get_name_0(self));
};
    font_asset.prototype['set_name'] = font_asset.prototype.set_name = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_font_asset_set_name_1(self, arg0);
};
    Object.defineProperty(font_asset.prototype, 'name', { get: font_asset.prototype.get_name, set: font_asset.prototype.set_name }) 
  font_asset.prototype['get_path'] = font_asset.prototype.get_path = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_font_asset_get_path_0(self));
};
    font_asset.prototype['set_path'] = font_asset.prototype.set_path = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_font_asset_set_path_1(self, arg0);
};
    Object.defineProperty(font_asset.prototype, 'path', { get: font_asset.prototype.get_path, set: font_asset.prototype.set_path }) 
  font_asset.prototype['__destroy__'] = font_asset.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_font_asset___destroy___0(self);
};
// asset_list
/** @suppress {undefinedVars, duplicate} */function asset_list() { throw "cannot construct a asset_list, no constructor in IDL" }
asset_list.prototype = Object.create(WrapperObject.prototype);
asset_list.prototype.constructor = asset_list;
asset_list.prototype.__class__ = asset_list;
asset_list.__cache__ = {};
Module['asset_list'] = asset_list;

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
  asset_list.prototype['get_numAssets'] = asset_list.prototype.get_numAssets = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_list_get_numAssets_0(self);
};
    asset_list.prototype['set_numAssets'] = asset_list.prototype.set_numAssets = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_list_set_numAssets_1(self, arg0);
};
    Object.defineProperty(asset_list.prototype, 'numAssets', { get: asset_list.prototype.get_numAssets, set: asset_list.prototype.set_numAssets }) 
  asset_list.prototype['get_maxAssets'] = asset_list.prototype.get_maxAssets = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_asset_list_get_maxAssets_0(self);
};
    asset_list.prototype['set_maxAssets'] = asset_list.prototype.set_maxAssets = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_list_set_maxAssets_1(self, arg0);
};
    Object.defineProperty(asset_list.prototype, 'maxAssets', { get: asset_list.prototype.get_maxAssets, set: asset_list.prototype.set_maxAssets }) 
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
  asset_to_load.prototype['get_image'] = asset_to_load.prototype.get_image = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_asset_to_load_get_image_0(self), image_asset);
};
    asset_to_load.prototype['set_image'] = asset_to_load.prototype.set_image = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_to_load_set_image_1(self, arg0);
};
    Object.defineProperty(asset_to_load.prototype, 'image', { get: asset_to_load.prototype.get_image, set: asset_to_load.prototype.set_image }) 
  asset_to_load.prototype['get_font'] = asset_to_load.prototype.get_font = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_asset_to_load_get_font_0(self), font_asset);
};
    asset_to_load.prototype['set_font'] = asset_to_load.prototype.set_font = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_asset_to_load_set_font_1(self, arg0);
};
    Object.defineProperty(asset_to_load.prototype, 'font', { get: asset_to_load.prototype.get_font, set: asset_to_load.prototype.set_font }) 
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
// image_asset
/** @suppress {undefinedVars, duplicate} */function image_asset() { throw "cannot construct a image_asset, no constructor in IDL" }
image_asset.prototype = Object.create(WrapperObject.prototype);
image_asset.prototype.constructor = image_asset;
image_asset.prototype.__class__ = image_asset;
image_asset.__cache__ = {};
Module['image_asset'] = image_asset;

  image_asset.prototype['get_id'] = image_asset.prototype.get_id = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_image_asset_get_id_0(self);
};
    image_asset.prototype['set_id'] = image_asset.prototype.set_id = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_image_asset_set_id_1(self, arg0);
};
    Object.defineProperty(image_asset.prototype, 'id', { get: image_asset.prototype.get_id, set: image_asset.prototype.set_id }) 
  image_asset.prototype['get_path'] = image_asset.prototype.get_path = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_image_asset_get_path_0(self));
};
    image_asset.prototype['set_path'] = image_asset.prototype.set_path = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_image_asset_set_path_1(self, arg0);
};
    Object.defineProperty(image_asset.prototype, 'path', { get: image_asset.prototype.get_path, set: image_asset.prototype.set_path }) 
  image_asset.prototype['__destroy__'] = image_asset.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_image_asset___destroy___0(self);
};
(function() {
  function setupEnums() {
    

    // asset_type

    Module['ASSET_TYPE_IMAGE'] = _emscripten_enum_asset_type_ASSET_TYPE_IMAGE();

    Module['ASSET_TYPE_FONT'] = _emscripten_enum_asset_type_ASSET_TYPE_FONT();

  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
