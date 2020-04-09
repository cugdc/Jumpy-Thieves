#ifndef JUMPY_THIEVES_GAME_HPP
#define JUMPY_THIEVES_GAME_HPP

#include <cstddef>

#include <entt/entt.hpp>

#include <beyond/math/point.hpp>
#include <beyond/math/vector.hpp>

#include "rect.hpp"
#include "texture.hpp"
#include "tile_map.hpp"

struct Position {
  beyond::Point2 data;
};

struct Velocity {
  beyond::Vec2 data;
};

struct Sprite {
  uint32_t tex_id = 0;
  Rect tex_coord{};
  float width = 0;
  float height = 0;
};

struct Player {
  float speed = 1;
};

class SpriteRenderer;

enum class Movement { stationary, up, down, left, right };

class Game {
  TileMap map_;
  Texture texture_;
  entt::registry registry_;
  entt::entity player_;

public:
  Game();

  auto set_player_movement(Movement movement) -> void;

  auto fixed_update() -> void;
  auto render(SpriteRenderer& sprite_renderer) -> void;
};

#endif // JUMPY_THIEVES_GAME_HPP
