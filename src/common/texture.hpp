#ifndef JUMPY_THIEVES_TEXTURE_HPP
#define JUMPY_THIEVES_TEXTURE_HPP

#include <beyond/types/optional.hpp>

#include <cstdint>

struct Texture {
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t id = 0;

  auto bind() -> void;
  auto unbind() -> void;
};

[[nodiscard]] auto load_texture(const char* file_path)
    -> beyond::optional<Texture>;

#endif // JUMPY_THIEVES_TEXTURE_HPP
