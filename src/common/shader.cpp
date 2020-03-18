#include <beyond/core/math/matrix.hpp>
#include <beyond/core/math/vector.hpp>

#include "file_util.hpp"
#include "shader.hpp"

Shader::Shader(const char* source, Shader::Type type) : type_{type}
{
  id_ = glCreateShader(static_cast<GLenum>(type));
  glShaderSource(id_, 1, &source, nullptr);
  glCompileShader(id_);
}

Shader::~Shader() noexcept
{
  glDeleteShader(id_);
}

Shader::Shader(Shader&& other) noexcept
    : type_{std::exchange(other.type_, {})}, id_{std::exchange(other.id_, {})}
{
}

auto Shader::operator=(Shader&& other) & noexcept -> Shader&
{
  type_ = std::exchange(other.type_, {});
  id_ = std::exchange(other.id_, {});
  return *this;
}

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders)
{
  id_ = glCreateProgram();
  for (const auto& shader : shaders) {
    glAttachShader(id_, shader.id_);
  }

  glLinkProgram(id_);
}

auto ShaderProgram::use() const -> void
{
  glUseProgram(id_);
}

[[nodiscard]] auto ShaderProgram::id() const -> unsigned int
{
  return id_;
}

auto ShaderBuilder::load(std::string_view filename, Shader::Type type)
    -> ShaderBuilder&
{
  const std::string src = read_file(filename);
  shaders_.emplace_back(src.c_str(), type);
  return *this;
}

[[nodiscard]] auto ShaderBuilder::build() const -> ShaderProgram
{
  return ShaderProgram{shaders_};
}

auto Shader::set_bool(const std::string& name, bool value) const noexcept
    -> void
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

auto Shader::set_int(const std::string& name, int value) const noexcept -> void
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

auto Shader::set_float(const std::string& name, float value) const noexcept
    -> void
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}
auto Shader::set_vec2(const std::string& name, const beyond::Vec2& value) const
    noexcept -> void
{
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto Shader::set_vec2(const std::string& name, float x, float y) const noexcept
    -> void
{
  glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}
auto Shader::set_vec3(const std::string& name, const beyond::Vec3& value) const
    noexcept -> void
{
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto Shader::set_vec3(const std::string& name, float x, float y, float z) const
    noexcept -> void
{
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}
auto Shader::set_vec4(const std::string& name, const beyond::Vec4& value) const
    noexcept -> void
{
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto Shader::set_vec4(const std::string& name, float x, float y, float z,
                      float w) const noexcept -> void
{
  glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}
auto Shader::set_mat2(const std::string& name, const beyond::Mat2& mat) const
    noexcept -> void
{
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
auto Shader::set_mat3(const std::string& name, const beyond::Mat3& mat) const
    noexcept -> void
{
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
auto Shader::set_mat4(const std::string& name, const beyond::Mat4& mat) const
    noexcept -> void
{
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
