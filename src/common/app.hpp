#pragma once

#ifndef JUMPY_THIEVES_APP_HPP
#define JUMPY_THIEVES_APP_HPP

#include <SDL2/SDL.h>
#include <array>
#include <chrono>

#include "shader.hpp"

using Milliseconds = std::chrono::duration<double, std::milli>;

class App {
  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_ = nullptr;
  bool running_ = true;

  ShaderProgram shader_program_;

  // clang-format off
  static constexpr std::array<float, 12> vertices_ = {0.0f, 1.0f,
                                                     1.0f, 0.0f,
                                                     0.0f, 0.0f,

                                                     0.0f, 1.0f,
                                                     1.0f, 1.0f,
                                                     1.0f, 0.0f
                                                     };
  // clang-format on
  uint32_t vbo_ = 0;

public:
  App();
  ~App();

  App(const App&) = delete;
  auto operator=(const App&) & noexcept = delete;
  App(App&&) noexcept = delete;
  auto operator=(App&&) & noexcept = delete;

  auto mainloop(const Milliseconds& delta_time) -> void;
  auto render(const Milliseconds& delta_time) -> void;
  auto handle_input() -> void;

  [[nodiscard]] auto window() const -> SDL_Window*
  {
    return window_;
  }

  [[nodiscard]] auto should_close() const -> bool
  {
    return !running_;
  }
};

#endif // JUMPY_THIEVES_APP_HPP
