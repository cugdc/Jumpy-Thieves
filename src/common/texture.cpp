#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fmt/format.h>

#include "opengl.hpp"

auto load_texture(const char* file_path) -> beyond::optional<Texture>
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
  if (data == nullptr) {
    fmt::print(stderr, "Error: cannot load texture {}\n", file_path);
    std::fflush(stderr);
    return {};
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture{.width = static_cast<uint32_t>(width),
                 .height = static_cast<uint32_t>(height),
                 .id = texture};
}

auto Texture::bind() -> void
{
  glBindTexture(GL_TEXTURE_2D, id);
}

auto Texture::unbind() -> void
{
  glBindTexture(GL_TEXTURE_2D, 0);
}