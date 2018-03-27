#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

bool Lexer::match(const string s)
{
    int i = 0;
    while (i < s.size()) {
        int c = fgetc(stdin);
        if (c == s[i]) {
            ++i;
        } else {
            ungetc(c, stdin);
            while (--i >= 0) {
                ungetc(s[i], stdin);
            }
            return false;
        }
    }
    return true;
}

unique_ptr<Token> Lexer::GetToken()
{
    char c = fgetc(stdin);
    while (isspace(c)) {
        c = fgetc(stdin);
    }
    if (isalpha(c)) {
        string id;
        id += c;
        c = fgetc(stdin);
        while (isalnum(c) || c == '_') {
            id += c;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        if (id == "if") {
            return unique_ptr<Token>(new Token(Tag::IF));
        } else if (id == "else") {
            return unique_ptr<Token>(new Token(Tag::ELSE));
        } else if (id == "do") {
            return unique_ptr<Token>(new Token(Tag::DO));
        } else if (id == "while") {
            return unique_ptr<Token>(new Token(Tag::WHILE));
        } else if (id == "break") {
            return unique_ptr<Token>(new Token(Tag::BREAK));
        } else if (id == "true") {
            return unique_ptr<Token>(new Token(Tag::TRUE));
        } else if (id == "false") {
            return unique_ptr<Token>(new Token(Tag::FALSE));
        }
        return unique_ptr<Token>(new Word(id));
    } else if (isdigit(c)) {
        int digit = 0;
        digit += (c - '0');
        while (isdigit(c = fgetc(stdin))) {
            digit *= 10;
            digit += (c - '0');
        }

        if (c == '.') {
            double real = digit;
            double w = 0.1;
            while (isdigit(c = fgetc(stdin))) {
                real += (c - '0') * w;
                w *= 0.1;
            }
            ungetc(c, stdin);
            // return real
            return unique_ptr<Token>(new Real(real));
        } else {
            ungetc(c, stdin);
            // return intger
            return unique_ptr<Token>(new Integer(digit));
        }
    } else {
        if (c == '&') {
            int n = fgetc(stdin);
            if (n == '&') {
                return unique_ptr<Token>(new Token(Tag::AND));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '|') {
            int n = fgetc(stdin);
            if (n == '|') {
                return unique_ptr<Token>(new Token(Tag::OR));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '>') {
            int n = fgetc(stdin);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::GE));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '<') {
            int n = fgetc(stdin);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::LE));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '=') {
            int n = fgetc(stdin);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::EQ));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '!') {
            int n = fgetc(stdin);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::NE));
            } else {
                ungetc(n, stdin);
                return unique_ptr<Token>(new Token(c));
            }
        } else {
            return unique_ptr<Token>(new Token(c));
        }
    }
}
