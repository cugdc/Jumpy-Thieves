
#include <fmt/format.h>

#include "game.hpp"
#include "opengl.hpp"
#include "rect.hpp"
#include "sprite_renderer.hpp"
#include "texture.hpp"

Game::Game() : map_{create_tile_map()}
{
  load_texture("assets/images/awesomeface.png")
      .map([this](const Texture& texture) { texture_ = texture; });

  player_ = registry_.create();
  registry_.assign<Position>(player_, beyond::Point2{400, 400});
  registry_.assign<Velocity>(player_, beyond::Vec2{0, 0});
  registry_.assign<Sprite>(player_, Sprite{texture_.id, {0, 0, 1, 1}, 50, 50});
  registry_.assign<Player>(player_, 1.5);

  for (int i = 0; i < 100; ++i) {
    const auto monster = registry_.create();
    const auto fi = static_cast<float>(i);
    registry_.assign<Position>(monster,
                               beyond::Point2{400.f + 50.f * std::sin(fi),
                                              400.f + 50.f * std::cos(fi)});
    registry_.assign<Velocity>(monster,
                               beyond::Vec2{std::cos(fi), std::sin(fi)});
    registry_.assign<Sprite>(monster,
                             Sprite{texture_.id, {0, 0, 1, 1}, 30, 30});
  }
}

auto Game::fixed_update() -> void
{
  registry_.view<Position, Velocity>().each(
      [&](Position& pos, const Velocity& vel) { pos.data += vel.data; });
}

auto Game::render(SpriteRenderer& sprite_renderer) -> void
{
  render_tile_map(map_, sprite_renderer);

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
