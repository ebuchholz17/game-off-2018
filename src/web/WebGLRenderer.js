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

    this.viewMatrix = null;
    this.projMatrix = null;
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

    setCamera: function (setCameraCommand) {
        this.viewMatrix = setCameraCommand.viewMatrix;
        this.projMatrix = setCameraCommand.projMatrix;
    },

    matrix4x4transpose: function (matrixBuffer) {
        var floatBuffer = new Float32Array(16)

        floatBuffer[0] = matrixBuffer[0];
        floatBuffer[1] = matrixBuffer[4];
        floatBuffer[2] = matrixBuffer[8];
        floatBuffer[3] = matrixBuffer[12];

        floatBuffer[4] = matrixBuffer[1];
        floatBuffer[5] = matrixBuffer[5];
        floatBuffer[6] = matrixBuffer[9];
        floatBuffer[7] = matrixBuffer[13];

        floatBuffer[8] = matrixBuffer[2];
        floatBuffer[9] = matrixBuffer[6];
        floatBuffer[10] = matrixBuffer[10];
        floatBuffer[11] = matrixBuffer[14];

        floatBuffer[12] = matrixBuffer[3];
        floatBuffer[13] = matrixBuffer[7];
        floatBuffer[14] = matrixBuffer[11];
        floatBuffer[15] = matrixBuffer[15]

        return floatBuffer;
    },

    drawMesh: function (game, meshCommand, program) {
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

        var floatBuffer = new Float32Array(game.buffer,
                                           this.viewMatrix.ptr,
                                           16); //4x4 matrix
        var viewMatrixLocation = gl.getUniformLocation(program, "viewMatrix");
        gl.uniformMatrix4fv(viewMatrixLocation, false, this.matrix4x4transpose(floatBuffer));

        floatBuffer = new Float32Array(game.buffer,
                                           this.projMatrix.ptr,
                                           16); //4x4 matrix
        var projMatrixLocation = gl.getUniformLocation(program, "projMatrix");
        gl.uniformMatrix4fv(projMatrixLocation, false, this.matrix4x4transpose(floatBuffer));

        gl.drawElements(gl.TRIANGLES, mesh.numIndices, gl.UNSIGNED_INT, 0);
    },

    renderFrame: function (game, renderCommands) {
        gl.viewport(0, 0, this.canvas.width, this.canvas.height);
        gl.enable(gl.DEPTH_TEST);
        //gl.enable(gl.CULL_FACE);
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
                    this.drawMesh(game, meshCommand, program);
                } break;
                case game.RENDER_COMMAND_SET_CAMERA:
                {
                    var setCameraCommand = game.wrapPointer(renderMemoryPointer + renderCommandOffset, 
                                                            game.render_command_set_camera);
                    renderCommandOffset += game.sizeof_render_command_set_camera();
                    this.setCamera(setCameraCommand, program);
                } break;
            }
        }
    }

};

module.exports = WebGLRenderer;
