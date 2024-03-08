//! code generation

#ifndef INTERPRETER_CODE_GEN_H
#define INTERPRETER_CODE_GEN_H

#pragma once

#include <AST/AST.h>
#include <AST/ASTVisitor.h>
#include <CodeGen/CodeGen.h>
#include <Interpreter/JIT.h>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Support/Error.h>

namespace toyc {

/**
 * @brief Codegen Visitor for interpreter
 *
 */
class InterpreterCodegenVisitor : public IRCodegenVisitor {
protected:
  std::unique_ptr<ToycJIT> JIT;
  std::unique_ptr<llvm::FunctionPassManager> FPM;
  std::unique_ptr<llvm::LoopAnalysisManager> LAM;
  std::unique_ptr<llvm::FunctionAnalysisManager> FAM;
  std::unique_ptr<llvm::CGSCCAnalysisManager> CGAM;
  std::unique_ptr<llvm::ModuleAnalysisManager> MAM;
  std::unique_ptr<llvm::PassInstrumentationCallbacks> PIC;
  std::unique_ptr<llvm::StandardInstrumentations> SI;
  llvm::ExitOnError ExitOnErr;

public:
  InterpreterCodegenVisitor();

  /**
   * Decl
   */

  virtual llvm::Value *codegen(const VarDecl &decl) override;
  // virtual llvm::Function *codegen(const FunctionDecl &decl) override;

  /**
   * TranslationUnitDecl
   */

  virtual void codegen(const TranslationUnitDecl &decl) override;
};

} // namespace toyc

#endif