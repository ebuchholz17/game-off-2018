var Game = require("./wasm/game.js");

var WebPlatform = function () {
    this.viewport = null;
    this.canvas = null;
    this.ctx = null;
    this.updateCallback = null;

    this.game = null;
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
        this.ctx = this.canvas.getContext("2d");

        window.addEventListener("resize", this.resize.bind(this));
        this.updateCallback = this.update.bind(this);

        this.assetList = this.game.wrapPointer(this.game._malloc(this.game.sizeof_asset_list()), 
                                               this.game.asset_list);
        this.assetList.set_numAssets(0);
        this.assetList.set_maxAssets(100);
        this.assetList.set_assetsToLoad(this.game._malloc(this.assetList.get_maxAssets() * this.game.sizeof_asset_to_load()));

        this.game.ccall("loadAssets", 
            "null", 
            ["number"], 
            [this.game.getPointer(this.assetList)]
        );

        var maxRectangles = 1000;
        this.rectangleList = this.game.wrapPointer(this.game._malloc(this.game.sizeof_rectangle_list()), 
                                                   this.game.rectangle_list);
        this.rectangleList.set_numRectangles(0);
        this.rectangleList.set_maxRectangles(1000);
        this.rectangleList.set_rectangles(this.game._malloc(this.rectangleList.get_maxRectangles() * this.game.sizeof_colored_rectangle()));

        this.resize();
        this.update();
    },

    initBackBuffer: function () {
    },

    update: function () {
        this.ctx.fillStyle = "#000000";
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

        this.rectangleList.set_numRectangles(0);
        this.game.ccall("updateGame", 
            "null", 
            ["number", "number", "number"], 
            [this.canvas.width, this.canvas.height, this.game.getPointer(this.rectangleList)]
        );
        
        var numRects = this.rectangleList.get_numRectangles();
        for (var i = 0; i < numRects; ++i) {
            var coloredRectangle = this.game.wrapPointer(this.game.getPointer(this.rectangleList.get_rectangles()) +
                                   i * this.game.sizeof_colored_rectangle(), this.game.colored_rectangle);
            var hexColor = coloredRectangle.get_color();
            this.ctx.fillStyle = this.hexColorToString(hexColor);
            this.ctx.fillRect(
                coloredRectangle.get_x(),
                coloredRectangle.get_y(),
                coloredRectangle.get_width(),
                coloredRectangle.get_height()
            );
        }

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
