"use strict";

var defaultVertexShaderSource = require("./shaders/defaultVertexShader.glsl");
var defaultFragmentShaderSource = require("./shaders/defaultFragmentShader.glsl");

var gl = null;

var WebGLMesh = function () {
    this.key = -1;
    this.positionBuffer = -1;
    this.texCoordBuffer = -1;
    this.normalBuffer= -1;
    this.indexBuffer= -1;
    this.numIndices = -1;
};

var ShaderProgram = function () {
    this.type = -1;
    this.vertexShader = -1;
    this.fragmentShader = -1;
    this.program = -1;
};

var ShaderTypes = {
    DEFAULT: 0
};

var WebGLRenderer = function () {
    this.shaders = [];
    this.meshes = [];
};

WebGLRenderer.prototype = {

    initWebGL: function (canvas) {
        this.canvas = canvas;
        gl = canvas.getContext("webgl");
        var success = gl.getExtension("OES_element_index_uint"); // TODO(ebuchholz): check
        this.compileAndLinkShader(defaultVertexShaderSource, defaultFragmentShaderSource, ShaderTypes.DEFAULT);
    },

    compileAndLinkShader: function (vertexShaderText, fragmentShaderText, type) {
        var shader = new ShaderProgram();
        shader.type = type;

        // TODO: factor compile code, whole shader build process, shader object
        shader.vertexShader = gl.createShader(gl.VERTEX_SHADER);
        gl.shaderSource(shader.vertexShader, vertexShaderText);
        gl.compileShader(shader.vertexShader);
        if (!gl.getShaderParameter(shader.vertexShader, gl.COMPILE_STATUS)) {
            console.log(gl.getShaderInfoLog(shader.vertexShader));
            return;
        }

        shader.fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
        gl.shaderSource(shader.fragmentShader, fragmentShaderText);
        gl.compileShader(shader.fragmentShader);
        if (!gl.getShaderParameter(shader.fragmentShader, gl.COMPILE_STATUS)) {
            console.log(gl.getShaderInfoLog(shader.fragmentShader));
            return;
        }

        shader.program = gl.createProgram();
        gl.attachShader(shader.program, shader.vertexShader);
        gl.attachShader(shader.program, shader.fragmentShader);
        gl.linkProgram(shader.program);

        if (!gl.getProgramParameter(shader.program, gl.LINK_STATUS)) {
            console.log("could not init shaders");
            return;
        }

        this.shaders.push(shader);
    },

    loadMesh: function (game, loadedMesh) {
        var mesh = new WebGLMesh();
        mesh.key = loadedMesh.key;
        this.meshes[mesh.key] = mesh;

        mesh.positionBuffer = gl.createBuffer();
        var floatBuffer = new Float32Array(game.buffer,
                                           loadedMesh.positions.values.ptr, 
                                           loadedMesh.positions.count);
        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.positionBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, floatBuffer, gl.STATIC_DRAW);

        mesh.texCoordBuffer = gl.createBuffer();
        floatBuffer = new Float32Array(game.buffer,
                                       loadedMesh.texCoords.values.ptr, 
                                       loadedMesh.texCoords.count);
        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.texCoordBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, floatBuffer, gl.STATIC_DRAW);

        mesh.normalBuffer = gl.createBuffer();
        floatBuffer = new Float32Array(game.buffer,
                                       loadedMesh.normals.values.ptr, 
                                       loadedMesh.normals.count);
        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.normalBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, floatBuffer, gl.STATIC_DRAW);

        mesh.indexBuffer = gl.createBuffer();
        var uintBuffer = new Uint32Array(game.buffer,
                                         loadedMesh.indices.values.ptr, 
                                         loadedMesh.indices.count);
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, uintBuffer, gl.STATIC_DRAW);

        mesh.numIndices = loadedMesh.indices.count;
    },

    drawMesh: function (meshCommand, program) {
        var mesh = this.meshes[meshCommand.key];

        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.positionBuffer);
        var positionLocation = gl.getAttribLocation(program, "position");
        gl.enableVertexAttribArray(positionLocation);
        gl.vertexAttribPointer(positionLocation, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.texCoordBuffer);
        var texCoordLocation = gl.getAttribLocation(program, "texCoord");
        gl.enableVertexAttribArray(texCoordLocation);
        gl.vertexAttribPointer(texCoordLocation, 2, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, mesh.normalBuffer);
        var normalLocation = gl.getAttribLocation(program, "normal");
        gl.enableVertexAttribArray(normalLocation);
        gl.vertexAttribPointer(normalLocation, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);

        gl.drawElements(gl.TRIANGLES, mesh.numIndices, gl.UNSIGNED_INT, 0);
    },

    renderFrame: function (game, renderCommands) {
        gl.viewport(0, 0, this.canvas.width, this.canvas.height);
        gl.enable(gl.DEPTH_TEST);
        gl.enable(gl.CULL_FACE);
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        var program = this.shaders[ShaderTypes.DEFAULT].program;
        gl.useProgram(program);

        var renderCommandOffset = 0;
        var renderCommandMemorySize = renderCommands.memory.size;
        var renderMemoryPointer = game.getPointer(renderCommands.memory.base);
        while (renderCommandOffset < renderCommandMemorySize) {
            var header = game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                               game.render_command_header);
            renderCommandOffset += game.sizeof_render_command_header();
            switch (header.type) {
                default:
                    break;
                case game.RENDER_COMMAND_MESH:
                {
                    var meshCommand = game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                                            game.render_mesh_command);
                    renderCommandOffset += game.sizeof_render_mesh_command();
                    this.drawMesh(meshCommand, program);
                } break;
            }
        }
    }

};

module.exports = WebGLRenderer;
