# Jumpy-Thieves
A 2D game.

## Build
### Emscripten
```shell
source <path to emsdk>/emsdk_env.sh
```

``` shell
conan install ./wasm.recipe.py --build missing --install-folder cmake-build-emscripten-release -pr ./wasm.profile
```
