var Game = require("./wasm/game.js");

var WebPlatform = function () {
    this.viewport = null;
    this.canvas = null;
    this.ctx = null;
    this.updateCallback = null;
    this.backBuffer = null;
    this.backBufferPixels = null;

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

        var bufferSize = 4 * this.canvas.width * this.canvas.height;

        this.backBufferPointer = this.game._malloc(bufferSize);
        this.backBufferPixels = new Uint8ClampedArray(this.game.HEAPU8.buffer, this.backBufferPointer, bufferSize);
        this.backBuffer = new ImageData(this.backBufferPixels, this.canvas.width, this.canvas.height);

        this.update();
    },

    initBackBuffer: function () {
    },

    update: function () {
        //console.log("updated");
        //this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        //for (var i = 0; i < this.canvas.height; ++i) {
        //    for (var j = 0; j < this.canvas.width; ++j) {

        //        this.backBufferPixels[(i * this.canvas.width + j) * 4] = 0x00;
        //        this.backBufferPixels[(i * this.canvas.width + j) * 4 + 1] = 0x00;
        //        this.backBufferPixels[(i * this.canvas.width + j) * 4 + 2] = 0x00;
        //        this.backBufferPixels[(i * this.canvas.width + j) * 4 + 3] = 0xff;
        //    }
        //}
        this.game.ccall("updateGame", 
            "null", 
            ["number", "number", "number"], 
            [this.canvas.width, this.canvas.height, this.backBufferPointer]
        );
        this.ctx.putImageData(this.backBuffer, 0, 0);

        window.requestAnimationFrame(this.updateCallback);
    },

    resize: function () {
        this.canvas.style.width = this.viewport.clientWidth + "px";
        this.canvas.style.height = this.viewport.clientHeight + "px";
    }

};

var platform = new WebPlatform();
platform.init();
