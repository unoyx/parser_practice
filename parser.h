#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "lexer.h"

class Parser
{
public:
    Parser(std::unique_ptr<Lexer> lex, llvm::Module *m, llvm::LLVMContext *c);
    void Parse();

private:
    llvm::Module *mModule;
    llvm::LLVMContext *mContext;
    std::unique_ptr<Lexer> mLex;
    std::unique_ptr<Token> mCurToken;

    void block(llvm::Function *f);
    void stmts();

    void move();
    bool match(Tag t);
    bool match(int c);
};

#endif
