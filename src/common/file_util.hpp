#pragma once

#ifndef JUMPY_THIEVES_FILE_UTIL_HPP
#define JUMPY_THIEVES_FILE_UTIL_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <spdlog/spdlog.h>

[[nodiscard]] auto read_file(std::string_view path) -> std::string;

#endif // JUMPY_THIEVES_FILE_UTIL_HPP
