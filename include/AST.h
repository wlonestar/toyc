//! AST

#ifndef AST_H
#define AST_H

#pragma once

#include <Token.h>

#include <llvm/IR/Value.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <variant>

namespace toyc {

enum Side {
  INTERNAL = 1,
  LEAF = 0,
};

/**
 * Expr
 */

class Expr {
public:
  virtual ~Expr() = default;

  virtual std::string getType() const = 0;
  virtual llvm::Value *codegen() = 0;
  virtual void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") = 0;
};

class Literal : public Expr {};

using integer_t = std::variant<int, unsigned int, long, unsigned long,
                               long long, unsigned long long>;

class IntegerLiteral : public Literal {
private:
  integer_t value;
  std::string type;

public:
  IntegerLiteral(integer_t _value, std::string &&_type)
      : value(_value), type(std::move(_type)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

using floating_t = std::variant<float, double, long double>;

class FloatingLiteral : public Literal {
private:
  floating_t value;
  std::string type;

public:
  FloatingLiteral(floating_t _value, std::string &&_type)
      : value(_value), type(std::move(_type)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class CharacterLiteral : public Literal {
private:
  int value;

public:
  CharacterLiteral(int _value) : value(_value) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class StringLiteral : public Literal {
private:
  std::string value;
  std::string type;

public:
  StringLiteral(std::string &&_value, std::string &&_type)
      : value(std::move(_value)), type(std::move(_type)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class DeclRefExpr : public Expr {
private:
  std::string type;
  std::string name;

public:
  DeclRefExpr(std::string &&_type, std::string &&_name)
      : type(std::move(_type)), name(std::move(_name)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class ParenExpr : public Expr {
private:
  std::unique_ptr<Expr> expr;

public:
  ParenExpr(std::unique_ptr<Expr> _expr) : expr(std::move(_expr)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class UnaryOperator : public Expr {
private:
  Token op;
  std::unique_ptr<Expr> right;

public:
  UnaryOperator(Token _op, std::unique_ptr<Expr> _right)
      : op(_op), right(std::move(_right)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

class BinaryOperator : public Expr {
private:
  Token op;
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  std::string type;

public:
  BinaryOperator(Token _op, std::unique_ptr<Expr> _left,
                 std::unique_ptr<Expr> _right, std::string &&_type)
      : op(_op), left(std::move(_left)), right(std::move(_right)),
        type(std::move(_type)) {}

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

/**
 * Stmt
 */

/**
 * Decl
 */

class Decl {
public:
  virtual ~Decl() = default;

  virtual std::string getType() const = 0;
  virtual llvm::Value *codegen() = 0;
  virtual void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") = 0;
};

class VarDecl : public Decl {
private:
  std::string type;
  std::string name;
  std::unique_ptr<Expr> init;

public:
  VarDecl(std::string &&_type, std::string &&_name,
          std::unique_ptr<Expr> _init = nullptr)
      : type(std::move(_type)), name(std::move(_name)), init(std::move(_init)) {
  }

  void setInit(std::unique_ptr<Expr> _init) { init = std::move(_init); }

  std::string getType() const override;
  llvm::Value *codegen() override;
  void dump(size_t _d = 0, Side _s = LEAF, std::string _p = "") override;
};

} // namespace toyc

#endif
