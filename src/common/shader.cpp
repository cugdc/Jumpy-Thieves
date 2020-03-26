#include <beyond/core/math/matrix.hpp>
#include <beyond/core/math/vector.hpp>

#include <iostream>
#include <vector>

#include "file_util.hpp"
#include "shader.hpp"

Shader::Shader(const char* source, Shader::Type type) : type_{type}
{
  id_ = glCreateShader(static_cast<GLenum>(type));
  glShaderSource(id_, 1, &source, nullptr);
  glCompileShader(id_);

  GLint isCompiled = 0;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(id_, maxLength, &maxLength, &errorLog[0]);

    std::cout << errorLog.data() << '\n';

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    glDeleteShader(id_); // Don't leak the shader.
    return;
  }
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

  int success;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  GLchar info_log[1024];
  if (!success) {
    glGetProgramInfoLog(id_, 1024, nullptr, static_cast<GLchar*>(info_log));
    std::cerr << fmt::format("Shader linking error: {}", info_log);
  }
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

auto ShaderProgram::set_bool(const std::string& name, bool value) const noexcept
    -> void
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

auto ShaderProgram::set_int(const std::string& name, int value) const noexcept
    -> void
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

auto ShaderProgram::set_float(const std::string& name, float value) const
    noexcept -> void
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}
auto ShaderProgram::set_vec2(const std::string& name,
                             const beyond::Vec2& value) const noexcept -> void
{
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto ShaderProgram::set_vec2(const std::string& name, float x, float y) const
    noexcept -> void
{
  glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}
auto ShaderProgram::set_vec3(const std::string& name,
                             const beyond::Vec3& value) const noexcept -> void
{
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto ShaderProgram::set_vec3(const std::string& name, float x, float y,
                             float z) const noexcept -> void
{
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}
auto ShaderProgram::set_vec4(const std::string& name,
                             const beyond::Vec4& value) const noexcept -> void
{
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value.elem.data());
}
auto ShaderProgram::set_vec4(const std::string& name, float x, float y, float z,
                             float w) const noexcept -> void
{
  glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}
auto ShaderProgram::set_mat2(const std::string& name,
                             const beyond::Mat2& mat) const noexcept -> void
{
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
auto ShaderProgram::set_mat3(const std::string& name,
                             const beyond::Mat3& mat) const noexcept -> void
{
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
auto ShaderProgram::set_mat4(const std::string& name,
                             const beyond::Mat4& mat) const noexcept -> void
{
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     mat.data);
}
