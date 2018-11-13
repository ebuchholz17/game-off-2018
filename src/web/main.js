var Game = require("./wasm/game.js");
var WebGLRenderer = require("./WebGLRenderer");

var WebPlatform = function () {
    this.viewport = null;
    this.canvas = null;
    this.updateCallback = null;

    this.game = null;
    this.renderer = null;
    this.totalAssetsLoaded = 0;
};

WebPlatform.prototype = {

    init: function () {
        this.game = Game();
        this.game.onRuntimeInitialized = this.run.bind(this);
    },

    run: function () {
        this.viewport = document.getElementById("run-around-game");
        this.canvas = document.createElement("canvas");
        this.viewport.insertAdjacentElement("beforeend", this.canvas);

        this.canvas.width = 1280;
        this.canvas.height = 720;

        window.addEventListener("resize", this.resize.bind(this));
        this.updateCallback = this.update.bind(this);

        this.renderer = new WebGLRenderer();
        this.renderer.initWebGL(this.canvas);

        this.gameMemory = this.game.wrapPointer(this.game._malloc(this.game.sizeof_game_memory()), 
                                                this.game.game_memory);
        this.gameMemory.memoryCapacity = 10 * 1024 * 1024;
        this.gameMemory.memory = this.game._malloc(this.gameMemory.memoryCapacity);

        this.assetList = this.game.wrapPointer(this.game._malloc(this.game.sizeof_asset_list()), 
                                                this.game.asset_list);
        this.assetList.numAssetsToLoad = 0;
        this.assetList.maxAssetsToLoad = 100;
        this.assetList.assetsToLoad = 
            this.game._malloc(this.assetList.maxAssetsToLoad * this.game.sizeof_asset_to_load());

        this.game.ccall("getGameAssetList", 
            "null", 
            ["number"], 
            [this.game.getPointer(this.assetList)]
        );

        this.workingAssetMemory = this.game.wrapPointer(this.game._malloc(this.game.sizeof_memory_arena()), 
                                                       this.game.memory_arena);
        this.workingAssetMemory.capacity = 10 * 1024 * 1024;
        this.workingAssetMemory.base = this.game._malloc(this.workingAssetMemory.capacity);

        var assetsToLoadPointer = this.game.getPointer(this.assetList.assetsToLoad);
        for (var i = 0; i < this.assetList.numAssetsToLoad; ++i) {
            var assetToLoad = this.game.wrapPointer(assetsToLoadPointer + this.game.sizeof_asset_to_load() * i, this.game.asset_to_load);
            switch (assetToLoad.type) {
                case this.game.ASSET_TYPE_OBJ: 
                {
                    fetch(assetToLoad.path).then(
                        function (file) {
                            file.text().then(
                                function (data) {


                                    var strLength = this.game.lengthBytesUTF8(data);
                                    var objFileData = this.game._malloc(strLength + 1);
                                    this.game.writeAsciiToMemory(data, objFileData); 

                                    this.workingAssetMemory.size = 0;

                                    this.game.ccall("parseGameAsset", 
                                        "null", 
                                        ["number", "number", "number", "number", "number"], 
                                        [
                                            objFileData, 
                                            assetToLoad.type, 
                                            assetToLoad.key, 
                                            this.game.getPointer(this.gameMemory),
                                            this.game.getPointer(this.workingAssetMemory)
                                        ]
                                    );

                                    var loadedMesh = 
                                        this.game.wrapPointer(
                                            this.game.getPointer(this.workingAssetMemory.base), 
                                            this.game.loaded_mesh_asset
                                        );

                                    this.renderer.loadMesh(this.game, loadedMesh);
                                    this.onAssetLoaded();

                                }.bind(this),
                                function () {
                                    console.log("fetch .text() failed for " + assetToLoad.path);
                                }.bind(this)
                            );
                        }.bind(this),
                        function () {
                            console.log("Failed to fetch " + assetToLoad.path);
                        }.bind(this)
                    );
                } break;
            }
        }

    },

    onAssetLoaded: function () {
        this.totalAssetsLoaded++;
        if (this.totalAssetsLoaded == this.assetList.numAssetsToLoad) {
            this.onAssetsLoaded();
        }
    },

    onAssetsLoaded: function () {
        this.game._free(this.game.getPointer(this.workingAssetMemory.base));
        this.renderCommands = this.game.wrapPointer(this.game._malloc(this.game.sizeof_render_command_list()), 
                                                   this.game.render_command_list);
        var renderCommandMemory = 1 * 1024 * 1024;
        this.renderCommands.get_memory().set_base(this.game._malloc(renderCommandMemory));
        this.renderCommands.get_memory().set_size(0);
        this.renderCommands.get_memory().set_capacity(renderCommandMemory);

        this.resize();
        this.update();
    },

    update: function () {

        this.renderCommands.get_memory().set_size(0);
        this.game.ccall("updateGame", 
            "null", 
            ["number"], 
            [this.game.getPointer(this.renderCommands)]
        );
        
        var renderCommandOffset = 0;
        var renderCommandMemorySize = this.renderCommands.get_memory().get_size();
        var renderMemoryPointer = this.game.getPointer(this.renderCommands.get_memory().get_base());
        while (renderCommandOffset < renderCommandMemorySize) {
            var header = this.game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                               this.game.render_command_header);
            renderCommandOffset += this.game.sizeof_render_command_header();
            switch (header.get_type()) {
                default:
                    break;
                case this.game.RENDER_COMMAND_RECTANGLE:
                {
                    var rectCommand = this.game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                                            this.game.render_rectangle_command);
                    renderCommandOffset += this.game.sizeof_render_rectangle_command();
                    //this.drawRectangle(rectCommand);
                } break;
                case this.game.RENDER_COMMAND_HORIZONTAL_LINE:
                {
                    var lineCommand = this.game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                                            this.game.render_horizontal_line_command);
                    renderCommandOffset += this.game.sizeof_render_horizontal_line_command();
                    //this.drawLine(lineCommand);
                } break;
            }
        }

        this.renderer.renderFrame(this.renderCommands);

        window.requestAnimationFrame(this.updateCallback);
    },

    hexColorToString: function (hexColor) {
        var a = ((hexColor >> 24) & 0xff).toString(16);
        var r = ((hexColor >> 16) & 0xff).toString(16);
        if (r.length == 1) { r = "0" + r; }
        var g = ((hexColor >> 8) & 0xff).toString(16);
        if (g.length == 1) { g = "0" + g; }
        var b = ((hexColor) & 0xff).toString(16);
        if (b.length == 1) { b = "0" + b; }
        return "#" + r + g + b;
    },

    resize: function () {
        this.canvas.style.width = this.viewport.clientWidth + "px";
        this.canvas.style.height = this.viewport.clientHeight + "px";
    }

};

var platform = new WebPlatform();
platform.init();
