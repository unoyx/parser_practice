#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include <utility>
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"

using namespace std;
using namespace llvm;

#ifdef DEBUG
#define debug_print(...)                   \
  do {                                          \
    fprintf(stdout, __VA_ARGS__);          \
  } while (0)
#else
#define debug_print(...)
#endif

Parser::Parser(std::unique_ptr<Lexer> lex, llvm::Module *m, llvm::LLVMContext *c)
    :mBuilder(new IRBuilder<>(*c)), mLex(std::move(lex)), mModule(m), mContext(c)
{
    move();
}

Parser::~Parser()
{
    delete mBuilder;
}

Tag Parser::getTokenTag()
{
    return mToken->GetTag();
}

int Parser::getToken()
{
    return mToken->GetToken();
}

string Parser::getWord()
{
    if (mToken->GetTag() == Tag::WORD) {
        auto w = static_cast<Word *>(mToken.get());
        auto ws = w->GetWord();
        return ws;
    }
    return "";
}

int Parser::getInteger()
{
    if (mToken->GetTag() == Tag::INTEGER) {
        auto i = static_cast<Integer *>(mToken.get());
        auto n = i->GetNum();
        return n;
    }
    return 0;
}

double Parser::getFloat()
{
    if (mToken->GetTag() == Tag::REAL) {
        auto r = static_cast<Real *>(mToken.get());
        auto f = r->GetReal();
        return f;
    }
    return 0.0;
}

void Parser::Parse()
{
    Function *mMain = cast<Function>(mModule->getOrInsertFunction("main", Type::getVoidTy(*mContext)));
    block(true);
}

void Parser::block(bool beginBlock)
{
    debug_print("enter block\n");
    consume('{');
    if (beginBlock) {
        BasicBlock *block = BasicBlock::Create(*mContext, "block", mMain);
        mBuilder->SetInsertPoint(block);
    }
    decls();
    stmts();
    consume('}');
}

void Parser::decls()
{
    debug_print("enter decls\n");
    while (getTokenTag() == Tag::INT ||
           getTokenTag() == Tag::FLOAT ||
           getTokenTag() == Tag::BOOL) {
        Type* t = type();
        auto s = getWord();
        AllocaInst *ins = mBuilder->CreateAlloca(t, 0, nullptr, s);
        mIDs.insert(make_pair(s, ins));
        move();
        consume(';');
    }
}

Type* Parser::type()
{
    debug_print("enter type\n");
    Type *t = nullptr;
    if (getTokenTag() == Tag::INT ||
        getTokenTag() == Tag::BOOL) {
        t = Type::getInt32Ty(*mContext);
    } else if (getTokenTag() == Tag::FLOAT) {
        t = Type::getFloatTy(*mContext);
    } else {
        return nullptr;
    }
    move();
    while (getTokenTag() == Tag::TOKEN && getToken() == '[') {
        move();
        if (getTokenTag() != Tag::INTEGER) {
            printf("expect integer number but find other token here.\n");
        }
        int n = getInteger();
        t = ArrayType::get(t, n);
        move();
        consume(']');
        move();
    }
    if (mToken->GetTag() != Tag::WORD) {
        printf("expect id here, but find other token here.\n");
    }
    return t;
}

void Parser::stmts()
{
    debug_print("enter stmts\n");
    while (mToken->GetTag() != Tag::TOKEN && mToken->GetToken() != '}') {
        stmt();
    }
}

void Parser::stmt()
{
    debug_print("enter stmt\n");
    BasicBlock *b = mBuilder->GetInsertBlock();
    if (getTokenTag() == Tag::IF) {
        consume(Tag::IF);
        consume('(');
        Value *boolVal = boolExp();
        consume(')');
        BasicBlock *trueBlock = BasicBlock::Create(*mContext, "true_part", mMain);
        mBuilder->SetInsertPoint(trueBlock);
        stmt();
        BasicBlock *endBlock = BasicBlock::Create(*mContext, "end_part", mMain);
        mBuilder->CreateBr(endBlock);
        if (getTokenTag() == Tag::ELSE) {
            BasicBlock *falseBlock = BasicBlock::Create(*mContext, "false_part", mMain);
            mBuilder->SetInsertPoint(falseBlock);
            stmt();
            BranchInst::Create(trueBlock, falseBlock, boolVal, b);
            mBuilder->SetInsertPoint(b);
            mBuilder->CreateCondBr(boolVal, trueBlock, falseBlock);
            mBuilder->SetInsertPoint(falseBlock);
            mBuilder->CreateBr(endBlock);
        }
    } else if (mToken->GetTag() == Tag::TOKEN) {
        if (mToken->GetToken() == '{') {
            block();
        }
    } else if (mToken->GetTag() == Tag::WORD) {
        AllocaInst *slot = location();
        consume('=');
        Value *n = boolExp();
        consume(';');
        mBuilder->CreateStore(n, slot);
    }
}

AllocaInst *Parser::location()
{
    debug_print("enter location\n");
    if (mToken->GetTag() == Tag::WORD) {
        auto s = getWord();
        if (mIDs.count(s) == 0) {
            printf("undefined variable %s.\n", s.c_str());
        }
        auto slot = mIDs[s];
        move();
        if (!match('[')) {
            return slot;
        } else {
            printf("currently we don't support array.\n");
            exit(1);
        }
    }
    return nullptr;
        /* just skip array process at this moment
        if (!var.IsArrayType()) {
            printf("can't reference %s by index.\n", s.c_str());
        }
        vector<Value*> array_pos;
        do {
            auto v = bool();
            array_pos.push_back(v);
            if (!match(']')) {
                printf("expect ] , but find other token %c here.\n", mToken->GetToken());
            }
            if (!match('[')) {
                return GetElementPtrInst::Create(var.GetPointerType(), var, array_pos, "", block);
            }
        } while (true);

    } else {
        printf("expect id, but find %c.\n", mToken->GetToken());
    }
        */
}

Value *Parser::boolExp()
{
    return factor();
}

Value *Parser::join()
{
}

Value *Parser::factor()
{
    if (getTokenTag() == Tag::INTEGER) {
        move();
        return ConstantInt::get(IntegerType::get(*mContext, 32), getInteger(), true);
    } else if (getTokenTag() == Tag::REAL) {
        move();
        return ConstantFP::get(*mContext, APFloat(getFloat()));
    } else if (getTokenTag() == Tag::TRUE) {
        move();
        return ConstantInt::getTrue(*mContext);
    } else if (getTokenTag() == Tag::FALSE) {
        move();
        return ConstantInt::getFalse(*mContext);
    } else if (getToken() == '(') {
        consume('(');
        auto ret = boolExp();
        consume(')');
        return ret;
    } else {
        return location();
    }
}

void Parser::move()
{
    mToken = mLex->GetToken();
    debug_print("%s\n", mToken->GetString().c_str());
}

bool Parser::match(Tag t)
{
    if (t == mToken->GetTag()) {
        move();
        return true;
    }
    return false;
}
bool Parser::match(int c)
{
    if (mToken->GetTag() == Tag::TOKEN && c == mToken->GetToken()) {
        move();
        return true;
    }
    return false;
}
bool Parser::match(string s)
{
    if (mToken->GetTag() == Tag::WORD) {
        auto w = static_cast<Word *>(mToken.get());
        auto ws = w->GetString();
        if (ws == s) {
            move();
        }
    }
    return false;
}

void Parser::consume(Tag t)
{
    if (!match(t)) {
        printf("expect tag: %d here, but find %s here.\n", (int)t, mToken->TagToString().c_str());
        assert(0);
    }
}

void Parser::consume(int c)
{
    if (!match(c)) {
        printf("expect token: %d here, but find %s here.\n", (int)c, mToken->TagToString().c_str());
        assert(0);
    }
}

void Parser::consume(string s)
{
    if (!match(s)) {
        printf("expect word: %s here, but find %s here.\n", s.c_str(), mToken->TagToString().c_str());
        assert(0);
    }
}
