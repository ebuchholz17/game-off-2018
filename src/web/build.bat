@echo off
IF NOT EXIST wasm mkdir wasm

emcc --bind -o wasm/game.js ../game/run_around_game.cpp bindings_wrapper.cpp -O2 -g4 -s WASM=1 -s MODULARIZE=1 -s EXPORTED_FUNCTIONS="['_updateGame']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" --post-js bindings.js
REM emcc -o wasm/game.js ../game/run_around_game.cpp -O0 -g4 -s WASM=1 -–source-map-base http://localhost:8080

