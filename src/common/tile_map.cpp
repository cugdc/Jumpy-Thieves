#include "tile_map.hpp"

TileMap::TileMap(std::vector<TileType> tile_types, TileGrid tiles,
                 Texture texture)
    : tile_types_{std::move(tile_types)}, tiles_{tiles}, texture_{texture}
{
}

[[nodiscard]] auto create_tile_map() -> TileMap
{
  TileMap::TileGrid tiles = {};
  tiles[0][0] = 1;
  tiles[0][1] = 2;
  tiles[1][1] = 2;
  tiles[2][1] = 3;
  tiles[3][2] = 21;

  Texture texture = load_texture("assets/images/tileset_town_multi_v002.png")
                        .value_or(Texture{});

  std::vector<TileType> tile_types;
  tile_types.reserve(texture.width / tile_width * texture.height / tile_height +
                     1);

  tile_types.push_back(TileType{});
  for (uint32_t y = 0; y < texture.height; y += tile_height) {
    for (uint32_t x = 0; x < texture.width; x += tile_width) {
      tile_types.push_back(
          TileType{.tex_coord_x = x / static_cast<float>(texture.width),
                   .tex_coord_y = y / static_cast<float>(texture.height)});
    }
  }

  return TileMap{tile_types, tiles, texture};
}

auto render_tile_map(const TileMap& tile_map, SpriteRenderer& sprite_renderer)
    -> void
{
  const auto texture = tile_map.texture_;

  for (uint32_t y = 0; y < tile_y_count; ++y) {
    for (uint32_t x = 0; x < tile_x_count; ++x) {
      const auto tile_type_id = tile_map.tiles_[y][x];

      if (tile_type_id == 0) {
        continue;
      }

      const auto tile_type = tile_map.tile_types_[tile_type_id];
      sprite_renderer.render(
          tile_map.texture_.id,
          Rect{static_cast<float>(x * tile_width),
               static_cast<float>(y * tile_height),
               static_cast<float>(tile_width), static_cast<float>(tile_height)},
          Rect{tile_type.tex_coord_x, tile_type.tex_coord_y,
               tile_width / static_cast<float>(texture.width),
               tile_height / static_cast<float>(texture.height)});
    }
  }
}