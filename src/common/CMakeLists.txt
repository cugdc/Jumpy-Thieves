add_library(common
        shader.cpp shader.hpp file_util.cpp file_util.hpp app.cpp app.hpp opengl.hpp
        sprite_renderer.hpp sprite_renderer.cpp game.hpp game.cpp
        rect.hpp texture.cpp texture.hpp tile_map.cpp tile_map.hpp constants.hpp)
target_link_libraries(common
        PUBLIC
        CONAN_PKG::spdlog
        CONAN_PKG::stb
        CONAN_PKG::entt
        imgui
        beyond::core
        PRIVATE compiler_options)
target_include_directories(common PUBLIC "${CMAKE_SOURCE_DIR}/include")

if (${JUMPY_THIEVES_BUILD_TESTS_COVERAGE})
    target_compile_options(common PUBLIC -fprofile-arcs -ftest-coverage)
    target_link_libraries(common PUBLIC gcov)
endif ()