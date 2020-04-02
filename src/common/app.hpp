#pragma once

#ifndef JUMPY_THIEVES_APP_HPP
#define JUMPY_THIEVES_APP_HPP

#include <SDL2/SDL.h>
#include <array>
#include <chrono>
#include <memory>

#include "game.hpp"
#include "shader.hpp"
#include "sprite_renderer.hpp"

using Milliseconds = std::chrono::duration<double, std::milli>;

class App {
  static constexpr int width = 1200;
  static constexpr int height = 800;

  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_ = nullptr;
  bool running_ = true;
  SpriteRenderer sprite_renderer_;
  Milliseconds remain_time_;

  std::unique_ptr<Game> game_;

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
