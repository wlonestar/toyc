//! toyc compiler class

#ifndef COMPILER_H
#define COMPILER_H

#pragma once

#include <compiler/CompilerCodeGen.h>
#include <compiler/CompilerParser.h>

namespace toyc {

/// acts as a compiler frontend: source code -> byte code
class Compiler {
private:
  CompilerParser parser;
  CompilerCodegenVisitor visitor;

private:
  bool readFrom(std::string &src, std::string &input);
  bool writeTo(std::string &dest);

public:
  Compiler() : parser(), visitor() {}

  /**
   * @brief compile source code to byte code (IR)
   *
   * @param src source code filepath
   * @param dest bytecode filepath
   */
  void compile(std::string &src, std::string &dest);
};

} // namespace toyc

#endif
