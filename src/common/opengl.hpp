#pragma once

#ifndef JUMPY_THIEVES_OPENGL_HPP
#define JUMPY_THIEVES_OPENGL_HPP

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#endif

#endif // JUMPY_THIEVES_OPENGL_HPP
