#ifndef UTIL_H
#define UTIL_H

#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

namespace toyc {

/**
 * @brief wrap a macro for providing a simple string formatter
 *
 */
// #define fstr(__fmt__, ...) fmt::format(__fmt__, ##__VA_ARGS__)

#define makeString(__fmt__, ...) fmt::format(__fmt__, ##__VA_ARGS__)

/**
 * @brief print info with located file and line number, for debugging
 *
 * @notice: only works on DEBUG mode
 */
// #ifdef DEBUG
#define debug(__fmt__, ...)                                                    \
  std::cerr << fmt::format("\033[1;34m{}:{} [debug] " __fmt__ "\033[0m\n",      \
                          __FILE__, __LINE__, ##__VA_ARGS__)
// #else
// #define debug(__fmt__, ...) ((void)0)
// #endif

static auto IsDigit(char c) -> bool { return (c >= '0' && c <= '9'); }

static auto IsNonZero(char c) -> bool { return (c >= '1' && c <= '9'); }

static auto IsLetter(char c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static auto IsAlpha(char c) -> bool { return IsLetter(c) || IsDigit(c); }

static auto StrContains(const std::string& str, char ch) -> bool {
  bool found = false;
  for (auto s : str) {
    if (ch == s) {
      found = true;
      break;
    }
  }
  return found;
}

/**
 * @brief read file content from `src` and write into string `input`
 *
 * @param src source file name
 * @param input content read from `src`
 * @return true if read and write successfully
 * @return false if file can not open
 */
static auto ReadFrom(const std::string &src, std::string &input) -> bool {
  std::ifstream file(src);
  if (!file.is_open()) {
    return false;
  }
  input = std::string((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  file.close();
  return true;
}

/**
 * @brief write content `output` into file `dest`
 *
 * @param dest destination file name
 * @param input content will be writtened
 * @return true if write into file successfully
 * @return false if file can not open
 */
static auto WriteTo(const std::string &dest, std::string &output) -> bool {
  std::ofstream file(dest);
  if (!file.is_open()) {
    return false;
  }
  file << output;
  file.close();
  return true;
}

static void Trim(std::string &str) {
  /// erase leading whitespaces
  size_t start = str.find_first_not_of(" \t\n\r\f\v");
  if (start != std::string::npos) {
    str.erase(0, start);
  } else {
    /// string is all whitespace
    str.clear();
    return;
  }
  /// erase trailing whitespaces
  size_t end = str.find_last_not_of(" \t\n\r\f\v");
  if (end != std::string::npos) {
    str.erase(end + 1);
  }
}

static auto CreateIfNotExist(const std::string &filePath) -> bool {
  std::ofstream file(filePath, std::ios::out | std::ios::app);
  if (!file.is_open()) {
    return false;
  }
  file.close();
  return true;
}

} // namespace toyc

#endif
