#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <memory>
#include "llvm/Support/Casting.h"

enum class Tag
{
    TOKEN = 1000,
    INTEGER,
    REAL,
    WORD,

    PARENTHESE, // ()
    BRACKET,    // []
    BRACE,      // {}
    IF,
    ELSE,
    WHILE,
    DO,
    BREAK,
    TRUE,
    FALSE,

    INT,
    FLOAT,
    BOOL,

    AND,
    OR,
    EQ,
    NE,
    GE,
    LE,
};

class Token
{
public:
    enum TokenKind
    {
        TK_TOKEN,
        TK_INTEGER,
        TK_REAL,
        TK_WORD,
    };
private:
    Tag mTag;
    int mToken;
    const TokenKind mKind;
public:
    TokenKind getKind() const
    {
        return mKind;
    }

    Token(int t, TokenKind kind = TK_TOKEN)
        :mTag(Tag::TOKEN), mToken(t), mKind(kind) {}

    Token(Tag t, TokenKind kind = TK_TOKEN)
        :mTag(t), mToken(0), mKind(kind) {}

    virtual ~Token() {}

    Tag GetTag()
    {
        return mTag;
    }

    int GetToken()
    {
        return mToken;
    }

    std::string TagToString()
    {
        std::string ret;
        ret += "[";
        switch (mTag)
        {
            case Tag::TOKEN:
                ret += "TOKEN";
                break;
            case Tag::INTEGER:
                ret += "INTEGER";
                break;
            case Tag::REAL:
                ret += "REAL";
                break;
            case Tag::WORD:
                ret += "WORD";
                break;
            case Tag::IF:
                ret += "IF";
                break;
            case Tag::ELSE:
                ret += "ELSE";
                break;
            case Tag::WHILE:
                ret += "WHILE";
                break;
            case Tag::DO:
                ret += "DO";
                break;
            case Tag::BREAK:
                ret += "BREAK";
                break;
            case Tag::TRUE:
                ret += "FALSE";
                break;
            case Tag::AND:
                ret += "AND";
                break;
            case Tag::OR:
                ret += "OR";
                break;
            case Tag::EQ:
                ret += "EQ";
                break;
            case Tag::NE:
                ret += "NE";
                break;
            case Tag::GE:
                ret += "GE";
                break;
            case Tag::LE:
                ret += "LE";
                break;
            default:
                break;
        }
        ret += "]";
        return ret;
    }

    std::string virtual GetString()
    {
        std::string ret = TagToString();
        ret += mToken;
        return ret;
    }

    static bool getClassOf(const Token *t)
    {
        return t->getKind() >= TK_TOKEN && t->getKind() <= TK_WORD;
    }
};

class Integer : public Token
{
private:
    int mNum;
public:
    Integer(int num)
        :Token(Tag::INTEGER, TK_INTEGER), mNum(num){}

    int GetNum()
    {
        return mNum;
    }

    std::string GetString() override
    {
        std::string ret = TagToString();
        ret += std::to_string(mNum);
        return ret;
    }

    static bool classof(const Token *t)
    {
        return t->getKind() >= TK_INTEGER;
    }
};

class Real : public Token
{
private:
    double mReal;
public:
    Real(double real)
        :Token(Tag::REAL, TK_REAL), mReal(real) {}

    double GetReal()
    {
        return mReal;
    }

    std::string virtual GetString() override
    {
        std::string ret = TagToString();
        ret += std::to_string(mReal);
        return ret;
    }

    static bool classof(const Token *t)
    {
        return t->getKind() >= TK_REAL;
    }
};

class Word : public Token
{
private:
    std::string mWord;
public:
    Word(std::string s)
        :Token(Tag::WORD, TK_WORD), mWord(s) {}

    std::string GetWord()
    {
        return mWord;
    }

    std::string virtual GetString() override
    {
        std::string ret = TagToString();
        ret += mWord;
        return ret;
    }

    static bool classof(const Token *t)
    {
        return t->getKind() >= TK_WORD;
    }
};

class Lexer
{
private:
    bool match(const std::string s);
    FILE *mInput;
public:
    Lexer():mInput(stdin) {}
    Lexer(char *filename)
    {
        FILE *f = fopen(filename, "r");
        mInput = f;
    }

    Lexer(const std::string &filename)
    {
        FILE *f = fopen(filename.c_str(), "r");
        mInput = f;
    }
    std::unique_ptr<Token> GetToken();
    ~Lexer()
    {
        if (mInput != stdin) {
            fclose(mInput);
        }
    }
};

#endif
