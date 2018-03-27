#include <stdio.h>
#include <stdlib.h>
#include "Lexer.h"

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


