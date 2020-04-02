#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fmt/format.h>

#include "game.hpp"
#include "opengl.hpp"
#include "rect.hpp"
#include "sprite_renderer.hpp"

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

Game::Game()
{
  texture_ = load_texture("assets/images/awesomeface.png");

  player_ = registry_.create();
  registry_.assign<Position>(player_, beyond::Point2{400, 400});
  registry_.assign<Velocity>(player_, beyond::Vec2{0, 0});
  registry_.assign<Sprite>(player_, Sprite{texture_, {0, 0, 1, 1}, 50, 50});
  registry_.assign<Player>(player_, 1.5);

  for (int i = 0; i < 100; ++i) {
    const auto monster = registry_.create();
    const auto fi = static_cast<float>(i);
    registry_.assign<Position>(monster,
                               beyond::Point2{400.f + 50.f * std::sin(fi),
                                              400.f + 50.f * std::cos(fi)});
    registry_.assign<Velocity>(monster,
                               beyond::Vec2{std::cos(fi), std::sin(fi)});
    registry_.assign<Sprite>(monster, Sprite{texture_, {0, 0, 1, 1}, 30, 30});
  }
}

auto Game::fixed_update() -> void
{
  registry_.view<Position, Velocity>().each(
      [&](Position& pos, const Velocity& vel) { pos.data += vel.data; });
}

auto Game::render(SpriteRenderer& sprite_renderer) -> void
{
  registry_.view<Position, Sprite>().each([&](const Position& pos,
                                              const Sprite& sprite) {
    sprite_renderer.render(
        sprite.tex_id, {pos.data.x, pos.data.y, sprite.width, sprite.height},
        sprite.tex_coord);
  });
}

auto Game::set_player_movement(Movement movement) -> void
{
  const auto speed = registry_.get<Player>(player_).speed;

  switch (movement) {
  case Movement::stationary:
    registry_.replace<Velocity>(player_, [&](Velocity& v) {
      v.data = beyond::Vec2{0, 0};
    });
    break;

  case Movement::left:
    registry_.replace<Velocity>(player_, [&](Velocity& v) {
      v.data = beyond::Vec2{-1, 0} * speed;
    });
    break;

  case Movement::right:
    registry_.replace<Velocity>(player_, [&](Velocity& v) {
      v.data = beyond::Vec2{1, 0} * speed;
    });
    break;

  case Movement::up:
    registry_.replace<Velocity>(player_, [&](Velocity& v) {
      v.data = beyond::Vec2{0, -1} * speed;
    });
    break;

  case Movement::down:
    registry_.replace<Velocity>(player_, [&](Velocity& v) {
      v.data = beyond::Vec2{0, 1} * speed;
    });
    break;
  }
}
