# Jumpy-Thieves
Jumpy-thieves is a 2d game live-coded in the CU Graphics & Game Dev Club.
You can see the video recording on [Youtube](https://www.youtube.com/playlist?list=PLlw1FcLpWd40erBuifv_PAMB9XpA11lL2).

## Build
### Emscripten
First, you need to bring emscripten in path.
```shell
source <path to emsdk>/emsdk_env.sh
```

Then you can `conan install` with the wasm recipe file.
``` shell
conan install ./wasm.recipe.py --build missing --install-folder cmake-build-emscripten-release -pr ./wasm.profile
```

You can continue with the normal CMake routine after the Conan setup.
