@echo off
IF NOT EXIST wasm mkdir wasm

emcc -o wasm/game.js ../game/run_around_game.cpp -O2 -s WASM=1 -s MODULARIZE=1 -s EXPORTED_FUNCTIONS="['_updateGame']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']"
REM emcc -o wasm/game.html ../game/run_around_game.cpp -O0 -s WASM=1

