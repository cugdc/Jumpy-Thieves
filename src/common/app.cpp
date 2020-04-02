#include "app.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <spdlog/spdlog.h>

#include <beyond/core/math/transform.hpp>

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

  game_ = std::make_unique<Game>();
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
  using namespace std::chrono_literals;
  handle_input();

  remain_time_ += delta_time;
  while (remain_time_ > 10ms) {
    game_->fixed_update();
    remain_time_ -= 10ms;
  }

  render(delta_time);
}

auto App::render(const Milliseconds & /*delta_time*/) -> void
{
  // Clear the screen
  glClearColor(48.f / 255, 10.f / 255, 36.f / 255, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  game_->render(sprite_renderer_);

  SDL_GL_SwapWindow(window_);
}

auto App::handle_input() -> void
{
  SDL_Event sdl_event;
  while (SDL_PollEvent(&sdl_event) != 0) {
    switch (sdl_event.type) {
    case SDL_KEYDOWN:
      switch (sdl_event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running_ = false;
        break;
      case SDLK_LEFT:
        game_->set_player_movement(Movement::left);
        break;
      case SDLK_RIGHT:
        game_->set_player_movement(Movement::right);
        break;
      case SDLK_UP:
        game_->set_player_movement(Movement::up);
        break;
      case SDLK_DOWN:
        game_->set_player_movement(Movement::down);
        break;
      default:
        break;
      }
      break;
    case SDL_KEYUP:
      switch (sdl_event.key.keysym.sym) {
      case SDLK_LEFT:
        [[fallthrough]];
      case SDLK_RIGHT:
        [[fallthrough]];
      case SDLK_UP:
        [[fallthrough]];
      case SDLK_DOWN:
        game_->set_player_movement(Movement::stationary);
        break;
      default:
        break;
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