#ifndef JUMPY_THIEVES_SPRITE_RENDERER_HPP
#define JUMPY_THIEVES_SPRITE_RENDERER_HPP

#include <array>
#include <cstddef>

#include "shader.hpp"

#include "beyond/core/math/matrix.hpp"

class SpriteRenderer {
  // clang-format off
  static constexpr std::array<float, 24> vertices = {
      // pos      texCoord
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f
  };
  // clang-format on
  ShaderProgram shader_program_;
  uint32_t vbo_ = 0;

public:
  SpriteRenderer() = default;
  explicit SpriteRenderer(const beyond::Mat4& projection);

  auto render(uint32_t texture, float x, float y) -> void;
};

#endif // JUMPY_THIEVES_SPRITE_RENDERER_HPP
