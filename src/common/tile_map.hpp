#ifndef JUMPY_THIEVES_TILE_MAP_HPP
#define JUMPY_THIEVES_TILE_MAP_HPP

#include "constants.hpp"
#include "sprite_renderer.hpp"
#include "texture.hpp"
#include <array>
#include <vector>

struct TileType {
  float tex_coord_x;
  float tex_coord_y;
};

class TileMap {
  std::vector<TileType> tile_types_;
  std::array<std::array<uint32_t, tile_x_count>, tile_y_count> tiles_{};
  Texture texture_;

public:
  using TileGrid = decltype(tiles_);

  explicit TileMap(std::vector<TileType> tile_types, TileGrid tiles,
                   Texture texture);

  friend auto render_tile_map(const TileMap& tile_map,
                              SpriteRenderer& sprite_renderer) -> void;

  friend auto create_tile_map() -> TileMap;
};

[[nodiscard]] auto create_tile_map() -> TileMap;

#endif // JUMPY_THIEVES_TILE_MAP_HPP
