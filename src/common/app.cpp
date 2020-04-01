#include "app.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <spdlog/spdlog.h>

#include <beyond/core/math/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

auto load_texture(const char* file_path) -> uint32_t
{
  uint32_t texture{};
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nv_channels;
  unsigned char* data = stbi_load(file_path, &width, &height, &nv_channels, 0);
  if (!data) {
    fmt::print(stderr, "Error: cannot load texture {}\n", file_path);
    std::fflush(stderr);
    return 0;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

App::App()
{
  static bool instantiated = false;
  if (instantiated) {
    spdlog::critical("The App class can only have one instance");
  }
  instantiated = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    spdlog::critical("[SDL2] Unable to initialize SDL: {}", SDL_GetError());
    // debug_break();
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL_GL_SetSwapInterval(1);

  window_ = SDL_CreateWindow("Jumpy Thieves", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, width, height,
                             SDL_WINDOW_OPENGL);
  if (window_ == nullptr) {
    spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
    std::exit(1);
  }

  gl_context_ = SDL_GL_CreateContext(window_);
  if (gl_context_ == nullptr) {
    spdlog::critical("[SDL2] OpenGL context is null: {}", SDL_GetError());
    std::exit(1);
  }

#ifndef __EMSCRIPTEN__
  if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0) {
    spdlog::critical("[Glad] Glad not init");
    std::exit(1);
  }
#endif

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);
  ImGui_ImplOpenGL3_Init("#version 300 es");
  ImGui::StyleColorsDark();

  const auto projection =
      beyond::ortho(0.f, static_cast<float>(width), static_cast<float>(height),
                    0.f, -1.f, 1.f);

  sprite_renderer_ = SpriteRenderer(projection);

  texture_ = load_texture("assets/images/awesomeface.png");
}

App::~App()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window_);
  SDL_Quit();
}

auto App::mainloop(const Milliseconds& delta_time) -> void
{
  handle_input();
  render(delta_time);
}

auto App::render(const Milliseconds & /*delta_time*/) -> void
{
  // Clear the screen
  glClearColor(48.f / 255, 10.f / 255, 36.f / 255, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  Rect dest{.width = 50, .height = 50, .x = 100, .y = 100};
  Rect textcoord{.width = 1, .height = 1, .x = 0, .y = 0};
  sprite_renderer_.render(texture_, dest, textcoord);

  dest.x = 300;
  dest.y = 300;
  textcoord.width = 0.5;
  textcoord.height = 0.5;
  sprite_renderer_.render(texture_, dest, textcoord);

  SDL_GL_SwapWindow(window_);
}

auto App::handle_input() -> void
{
  SDL_Event sdl_event;
  while (SDL_PollEvent(&sdl_event) != 0) {
    switch (sdl_event.type) {
    case SDL_KEYDOWN:
      if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
        running_ = false;
      }
      break;
    case SDL_QUIT:
      running_ = false;
      break;
    default:
      break;
    }
  }
}