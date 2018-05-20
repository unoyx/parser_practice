#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <map>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Instructions.h"
#include "lexer.h"

class Parser
{
public:
    Parser(Lexer *lex);
    void Parse();
    ~Parser();
    void Dump()
    {
        mModule->dump();
    }

private:
    llvm::LLVMContext mContext;
    llvm::Module *mModule;
    llvm::IRBuilder<> *mBuilder;
    llvm::Function *mMain;
    Lexer *mLex;
    std::unique_ptr<Token> mToken;
    std::map<std::string, llvm::AllocaInst*> mIDs;

    void block(bool beginBlock = false);
    void decls();
    llvm::Type *type();

    void stmts();
    void stmt();
    llvm::AllocaInst *location();
    llvm::Value *boolExp();
    llvm::Value *join();
    llvm::Value *factor();

    void move();

    Tag getTokenTag();
    int getToken();
    std::string getWord();
    int getInteger();
    double getFloat();

    bool match(Tag t);
    bool match(int c);
    bool match(std::string s);

    void consume(Tag t);
    void consume(int c);
    void consume(std::string s);
};

#endif
