#include "parser.h"
#include <stdio.h>
#include <utility>
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"

using namespace std;
using namespace llvm;

Parser::Parser(std::unique_ptr<Lexer> lex, llvm::Module *m, llvm::LLVMContext *c)
    :mLex(std::move(lex)), mModule(m), mContext(c)
{
    move();
}

void Parser::Parse()
{
    Function *main = cast<Function>(mModule->getOrInsertFunction("main", Type::getVoidTy(*mContext)));
    block(main);
}

void Parser::block(Function *f)
{
    if (!match('{')) {
        printf("expect {, but find %c. \n", mCurToken->GetToken());
        return;
    }
    BasicBlock *block = BasicBlock::Create(*mContext, "block", f);

    stmts();
    if (!match('}')) {
        printf("expect }, but find %c. \n", mCurToken->GetToken());
        return;
    }

}

void Parser::stmts()
{}


void Parser::move()
{
    mCurToken = mLex->GetToken();
}

bool Parser::match(Tag t)
{
    if (t == mCurToken->GetTag()) {
        move();
        return true;
    }
    return false;
}
bool Parser::match(int c)
{
    if (mCurToken->GetTag() == Tag::TOKEN && c == mCurToken->GetToken()) {
        move();
        return true;
    }
    return false;
}
