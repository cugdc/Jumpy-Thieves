add_library(imgui STATIC
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_sdl.h
        imgui/imgui_impl_sdl.cpp)

target_link_libraries(imgui
        PUBLIC
        glad
        CONAN_PKG::imgui
        )
target_include_directories(imgui PUBLIC imgui)

add_library(glad STATIC
        glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_subdirectory(beyond-core)