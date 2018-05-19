#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

bool Lexer::match(const string s)
{
    int i = 0;
    while (i < s.size()) {
        int c = fgetc(mInput);
        if (c == s[i]) {
            ++i;
        } else {
            ungetc(c, mInput);
            while (--i >= 0) {
                ungetc(s[i], mInput);
            }
            return false;
        }
    }
    return true;
}

unique_ptr<Token> Lexer::GetToken()
{
    char c = fgetc(mInput);
    while (isspace(c)) {
        c = fgetc(mInput);
    }
    if (isalpha(c)) {
        string id;
        id += c;
        c = fgetc(mInput);
        while (isalnum(c) || c == '_') {
            id += c;
            c = fgetc(mInput);
        }
        ungetc(c, mInput);
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
        } else if (id == "int") {
            return unique_ptr<Token>(new Token(Tag::INT));
        } else if (id == "float") {
            return unique_ptr<Token>(new Token(Tag::FLOAT));
        } else if (id == "bool") {
            return unique_ptr<Token>(new Token(Tag::BOOL));
        }
        return unique_ptr<Token>(new Word(id));
    } else if (isdigit(c)) {
        int digit = 0;
        digit += (c - '0');
        while (isdigit(c = fgetc(mInput))) {
            digit *= 10;
            digit += (c - '0');
        }

        if (c == '.') {
            double real = digit;
            double w = 0.1;
            while (isdigit(c = fgetc(mInput))) {
                real += (c - '0') * w;
                w *= 0.1;
            }
            ungetc(c, mInput);
            // return real
            return unique_ptr<Token>(new Real(real));
        } else {
            ungetc(c, mInput);
            // return intger
            return unique_ptr<Token>(new Integer(digit));
        }
    } else {
        if (c == '&') {
            int n = fgetc(mInput);
            if (n == '&') {
                return unique_ptr<Token>(new Token(Tag::AND));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '|') {
            int n = fgetc(mInput);
            if (n == '|') {
                return unique_ptr<Token>(new Token(Tag::OR));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '>') {
            int n = fgetc(mInput);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::GE));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '<') {
            int n = fgetc(mInput);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::LE));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '=') {
            int n = fgetc(mInput);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::EQ));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else if (c == '!') {
            int n = fgetc(mInput);
            if (n == '=') {
                return unique_ptr<Token>(new Token(Tag::NE));
            } else {
                ungetc(n, mInput);
                return unique_ptr<Token>(new Token(c));
            }
        } else {
            return unique_ptr<Token>(new Token(c));
        }
    }
}
