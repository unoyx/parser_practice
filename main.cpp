#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
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
int main(int argc, char *argv[])
{
    LLVMContext context;
    Module module("major", context);

    Lexer *lexer = nullptr;
    if (argc > 1) {
        lexer = new Lexer(argv[1]);
    } else {
        lexer = new Lexer();
    }
    Parser parser(unique_ptr<Lexer>(lexer), &module, &context);

    parser.Parse();

    module.print(errs(), nullptr);
    // module.dump();

    return 0;
}
#endif
