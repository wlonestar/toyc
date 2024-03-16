//! parser of toyc

#ifndef PARSER_H
#define PARSER_H

#pragma once

#include <AST/AST.h>
#include <Lexer/Lexer.h>

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <tuple>
#include <vector>

namespace toyc {

class ParserException : public std::exception {
private:
  size_t line;
  size_t col;
  std::string message;

public:
  ParserException(size_t _line, size_t _col, std::string _msg)
      : line(_line), col(_col),
        message(fstr("\033[1;37mline:{}:col:{}:\033[0m "
                     "\033[1;31merror:\033[0m \033[1;37m{}\033[0m",
                     _line, _col, _msg)) {}

  const char *what() const noexcept override { return message.c_str(); }
};

struct FunctionParams {
  std::string name;
  std::string retType;
  std::vector<std::string> params;

  FunctionParams() {}

  FunctionParams(std::string _name, std::string _retType,
                 std::vector<std::string> &_params)
      : name(_name), retType(_retType), params(_params) {}
};

class BaseParser {
protected:
  Token current;
  Token prev;
  Lexer lexer;

protected:
  /// global variable: <name, type>
  std::map<std::string, std::string> globalVarTable;
  /// local variable: <name, type>
  std::map<std::string, std::string> varTable;
  /// function declaration: <name, pair<retType, [type]...>>
  std::map<std::string, FunctionParams> funcTable;

protected:
  void throwParserException(std::string message) {
    throw ParserException(current.line, current.col, std::move(message));
  }

protected:
  void clearVarTable() { varTable.clear(); }

public:
  Token peek() { return current; }
  Token previous() { return prev; }
  Token advance();
  Token consume(TokenType type, std::string &message);
  Token consume(TokenType type, std::string &&message);

protected:
  bool check(std::initializer_list<TokenType> types);
  bool check(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  bool match(TokenType type);

protected:
  bool checkHexadecimal(std::string &value);
  bool checkOctal(std::string &value);

  int64_t parseIntegerSuffix(std::string &value, int base);
  double parseFloatingSuffix(std::string &value, int base);

  std::string checkUnaryOperatorType(TokenType type, Expr *right);
  std::string checkBinaryOperatorType(TokenType type,
                                      std::unique_ptr<Expr> &left,
                                      std::unique_ptr<Expr> &right);
  std::string checkShiftOperatorType(TokenType type,
                                     std::unique_ptr<Expr> &left,
                                     std::unique_ptr<Expr> &right);

protected:
  std::unique_ptr<Expr> parseIntegerLiteral();
  std::unique_ptr<Expr> parseFloatingLiteral();
  std::unique_ptr<Expr> parseConstant();
  std::unique_ptr<Expr> parsePrimaryExpression();
  std::unique_ptr<Expr> parsePostfixExpression();
  std::unique_ptr<Expr> parseUnaryExpression();
  std::unique_ptr<Expr> parseMultiplicativeExpression();
  std::unique_ptr<Expr> parseAdditiveExpression();
  std::unique_ptr<Expr> parseShiftExpression();
  std::unique_ptr<Expr> parseRelationalExpression();
  std::unique_ptr<Expr> parseEqualityExpression();
  std::unique_ptr<Expr> parseLogicalAndExpression();
  std::unique_ptr<Expr> parseLogicalOrExpression();
  std::unique_ptr<Expr> parseAssignmentExpression();
  std::unique_ptr<Expr> parseExpression();

public:
  std::unique_ptr<Stmt> parseExpressionStatement();
  std::unique_ptr<Stmt> parseReturnStatement();
  std::unique_ptr<Stmt> parseIterationStatement();
  std::unique_ptr<Stmt> parseSelectionStatement();
  std::unique_ptr<Stmt> parseDeclarationStatement();
  std::unique_ptr<Stmt> parseCompoundStatement();
  std::unique_ptr<Stmt> parseStatement();

public:
  std::pair<std::string, bool> parseDeclarationSpecifiers();
  std::string parseDeclarator();
  std::vector<std::unique_ptr<ParmVarDecl>> parseFunctionParameters();
  std::string genFuncType(std::string &&retTy,
                          std::vector<std::unique_ptr<ParmVarDecl>> &params);

public:
  virtual std::unique_ptr<Decl> parseVariableDeclaration(std::string &type,
                                                         std::string &name,
                                                         VarScope scope);
  std::unique_ptr<Decl>
  parseFunctionDeclaration(std::string &type, std::string &name, bool isExtern);

  std::unique_ptr<Decl> parseExternalDeclaration();

public:
  BaseParser() : lexer(), current(), prev() {}

public:
  void addInput(std::string &_input) { lexer.addInput(_input); }
  std::string getInput() { return lexer.getInput(); }
};

class Parser : public BaseParser {
public:
  Parser() : BaseParser() {}

public:
  std::unique_ptr<TranslationUnitDecl> parse();
};

} // namespace toyc

#endif
