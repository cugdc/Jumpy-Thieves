#include "sprite_renderer.hpp"
#include "beyond/core/math/transform.hpp"

#include <SDL2/SDL_opengles2.h>

SpriteRenderer::SpriteRenderer(const beyond::Mat4& projection)
{

  shader_program_ =
      ShaderBuilder()
          .load("assets/shaders/sprite.vert.glsl", Shader::Type::Vertex)
          .load("assets/shaders/sprite.frag.glsl", Shader::Type::Fragment)
          .build();
  shader_program_.use();
  shader_program_.set_mat4("projection", projection);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  std::uint32_t ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);

  GLint pos_attrib = glGetAttribLocation(shader_program_.id(), "vertices");
  glEnableVertexAttribArray(pos_attrib);
  glVertexAttribPointer(pos_attrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

struct Vertex {
  beyond::Vec2 pos;
  beyond::Vec2 tex_coord;
};

auto SpriteRenderer::render(uint32_t texture, const Rect& dest,
                            const Rect& tex_coord) -> void
{
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  // clang-format off
  std::array vertices = {
      Vertex{{dest.x, dest.y + dest.height}, {tex_coord.x, tex_coord.y + tex_coord.height}},
      Vertex{{dest.x, dest.y}, {tex_coord.x, tex_coord.y}},
      Vertex{{dest.x + dest.width, dest.y}, {tex_coord.x + tex_coord.width, tex_coord.y}},
      Vertex{{dest.x + dest.width, dest.y + dest.height}, {tex_coord.x + tex_coord.width, tex_coord.y + tex_coord.height}},
  };
  // clang-format on

  shader_program_.use();

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
