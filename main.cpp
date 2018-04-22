#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

#ifdef LEXER_TEST
int main()
{
    Lexer lex;

    unique_ptr<Token> token = lex.GetToken();
    while (token->GetToken() != EOF) {
        string s = token->GetString();
        printf("%s\n", s.c_str());
        token = lex.GetToken();
    }
    return 0;
}
#endif


#ifdef PARSER_TEST
using namespace llvm;
int main()
{
    LLVMContext context;
    Module module("major", context);

    Parser parser(make_unique<Lexer>(), &module, &context);
    parser.Parse();

    module.dump();

    return 0;
}
#endif
