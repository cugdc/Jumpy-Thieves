#ifndef JUMPY_THIEVES_CONSTANTS_HPP
#define JUMPY_THIEVES_CONSTANTS_HPP

#include <cstdint>

static constexpr uint32_t tile_width = 32;
static constexpr uint32_t tile_height = 32;
static constexpr uint32_t tile_x_count = 40;
static constexpr uint32_t tile_y_count = 30;
static constexpr uint32_t window_width = tile_width * tile_x_count;
static constexpr uint32_t window_height = tile_height * tile_y_count;

#endif // JUMPY_THIEVES_CONSTANTS_HPP
