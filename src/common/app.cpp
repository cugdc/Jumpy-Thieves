#include "app.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <spdlog/spdlog.h>

#include <beyond/core/math/transform.hpp>

constexpr int width = 1200;
constexpr int height = 800;

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

  shader_program_ =
      ShaderBuilder()
          .load("assets/shaders/triangle.vert.glsl", Shader::Type::Vertex)
          .load("assets/shaders/triangle.frag.glsl", Shader::Type::Fragment)
          .build();
  shader_program_.use();

  const auto projection =
      beyond::ortho(0.f, static_cast<float>(width), static_cast<float>(height),
                    0.f, -1.f, 1.f);
  shader_program_.set_mat4("projection", projection);
  shader_program_.use();

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(),
               GL_STATIC_DRAW);

  GLint pos_attrib = glGetAttribLocation(shader_program_.id(), "position");
  glEnableVertexAttribArray(pos_attrib);
  glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
  using namespace beyond::literals;

  // Clear the screen
  glClearColor(48.f / 255, 10.f / 255, 36.f / 255, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  beyond::Vec2 size{50.f, 50.f};

  const auto model = beyond::translate(100.f, 100.f, 0.f) * // Position
                     beyond::translate(0.5f * size.x, 0.5f * size.y, 0.f) *
                     beyond::rotate_z(45._deg) *
                     beyond::translate(-0.5f * size.x, -0.5f * size.y, 0.f) *
                     beyond::scale(size.x, size.y, 1.f);
  shader_program_.set_mat4("model", model);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(),
               GL_DYNAMIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / 2);

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
    }
  }
}