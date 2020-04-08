#ifndef JUMPY_THIEVES_SPRITE_RENDERER_HPP
#define JUMPY_THIEVES_SPRITE_RENDERER_HPP

#include <array>
#include <cstddef>

#include "rect.hpp"
#include "shader.hpp"

#include "beyond/math/matrix.hpp"

class SpriteRenderer {
  static constexpr std::array<uint32_t, 6> indices = {0, 1, 3, 1, 2, 3};

  ShaderProgram shader_program_;
  uint32_t vbo_ = 0;
  uint32_t vao_ = 0;

public:
  SpriteRenderer() = default;
  explicit SpriteRenderer(const beyond::Mat4& projection);

  auto render(uint32_t texture, const Rect& dest,
              const Rect& tex_coord = {0, 0, 1, 1}) -> void;
};

#endif // JUMPY_THIEVES_SPRITE_RENDERER_HPP
