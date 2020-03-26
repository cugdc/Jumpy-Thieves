#include "sprite_renderer.hpp"
#include "beyond/core/math/transform.hpp"

#include <SDL2/SDL_opengles2.h>

SpriteRenderer::SpriteRenderer(const beyond::Mat4& projection)
{

  shader_program_ =
      ShaderBuilder()
          .load("assets/shaders/triangle.vert.glsl", Shader::Type::Vertex)
          .load("assets/shaders/triangle.frag.glsl", Shader::Type::Fragment)
          .build();
  shader_program_.use();

  shader_program_.set_mat4("projection", projection);
  shader_program_.use();

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  GLint pos_attrib = glGetAttribLocation(shader_program_.id(), "position");
  glEnableVertexAttribArray(pos_attrib);
  glVertexAttribPointer(pos_attrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto SpriteRenderer::render(uint32_t texture, float x, float y) -> void
{
  using namespace beyond::literals;

  beyond::Vec2 size{50.f, 50.f};

  const auto model = beyond::translate(x, y, 0.f) * // Position
                     beyond::translate(0.5f * size.x, 0.5f * size.y, 0.f) *
                     beyond::rotate_z(0._deg) *
                     beyond::translate(-0.5f * size.x, -0.5f * size.y, 0.f) *
                     beyond::scale(size.x, size.y, 1.f);
  shader_program_.set_mat4("model", model);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindTexture(0, texture);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
