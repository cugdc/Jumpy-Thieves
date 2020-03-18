#pragma once

#ifndef JUMPY_THIEVES_SHADER
#define JUMPY_THIEVES_SHADER

#include <spdlog/spdlog.h>

#include <string_view>
#include <vector>

#include <beyond/core/math/math_fwd.hpp>

#include "opengl.hpp"

class Shader {
public:
  enum class Type : GLenum {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
  };

private:
  Type type_;
  unsigned int id_;

public:
  Shader(const char* source, Type type);

  ~Shader() noexcept;

  Shader(const Shader& other) = delete;
  auto operator=(const Shader& other) & -> Shader& = delete;
  Shader(Shader&& other) noexcept;
  auto operator=(Shader&& other) & noexcept -> Shader&;

  auto set_bool(const std::string& name, bool value) const noexcept -> void;

  auto set_int(const std::string& name, int value) const noexcept -> void;
  auto set_float(const std::string& name, float value) const noexcept -> void;

  auto set_vec2(const std::string& name, const beyond::Vec2& value) const
      noexcept -> void;
  auto set_vec2(const std::string& name, float x, float y) const noexcept
      -> void;

  auto set_vec3(const std::string& name, const beyond::Vec3& value) const
      noexcept -> void;

  auto set_vec3(const std::string& name, float x, float y, float z) const
      noexcept -> void;

  auto set_vec4(const std::string& name, const beyond::Vec4& value) const
      noexcept -> void;

  auto set_vec4(const std::string& name, float x, float y, float z,
                float w) const noexcept -> void;
  auto set_mat2(const std::string& name, const beyond::Mat2& mat) const noexcept
      -> void;
  auto set_mat3(const std::string& name, const beyond::Mat3& mat) const noexcept
      -> void;
  auto set_mat4(const std::string& name, const beyond::Mat4& mat) const noexcept
      -> void;

  friend class ShaderProgram;
};

class ShaderProgram {
  unsigned int id_{};

public:
  ShaderProgram() = default;
  explicit ShaderProgram(const std::vector<Shader>& shaders);

  auto use() const -> void;
  [[nodiscard]] auto id() const -> unsigned int;
};

class ShaderBuilder {
public:
  ShaderBuilder() = default;

  auto load(std::string_view filename, Shader::Type type) -> ShaderBuilder&;

  [[nodiscard]] auto build() const -> ShaderProgram;

private:
  std::vector<Shader> shaders_;
};

#endif // JUMPY_THIEVES_SHADER