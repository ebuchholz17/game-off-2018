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


        this.renderCommands = this.game.wrapPointer(this.game._malloc(this.game.sizeof_render_command_list()), 
                                                   this.game.render_command_list);
        var renderMemory = 1 * 1024 * 1024;
        this.renderCommands.get_memory().set_base(this.game._malloc(renderMemory));
        this.renderCommands.get_memory().set_size(0);
        this.renderCommands.get_memory().set_capacity(renderMemory);

        this.resize();
        this.update();
    },

    initBackBuffer: function () {
    },

    drawRectangle: function (rectCommand) {
        this.ctx.fillStyle = this.hexColorToString(rectCommand.get_color());
        this.ctx.fillRect(
            rectCommand.get_x(),
            rectCommand.get_y(),
            rectCommand.get_width(),
            rectCommand.get_height()
        );
    },
    drawLine: function (lineCommand) {
        this.ctx.strokeStyle = this.hexColorToString(lineCommand.get_color());
        var lineNum = lineCommand.get_lineNum();
        this.ctx.beginPath();
        this.ctx.moveTo(0, lineNum);
        this.ctx.lineTo(this.canvas.width - 1, lineNum);
        this.ctx.stroke();
    },

    update: function () {
        this.ctx.fillStyle = "#000000";
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

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
                    this.drawRectangle(rectCommand);
                } break;
                case this.game.RENDER_COMMAND_HORIZONTAL_LINE:
                {
                    var lineCommand = this.game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                                            this.game.render_horizontal_line_command);
                    renderCommandOffset += this.game.sizeof_render_horizontal_line_command();
                    this.drawLine(lineCommand);
                } break;
            }
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
