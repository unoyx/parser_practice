#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <memory>

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

    AND,
    OR,
    EQ,
    NE,
    GE,
    LE,
};

class Token
{
private:
    Tag mTag;
    int mToken;
public:
    Token(int t)
        :mTag(Tag::TOKEN), mToken(t) {}

    Token(Tag t)
        :mTag(t), mToken(0) {}

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
};

class Integer : public Token
{
private:
    int mNum;
public:
    Integer(int num)
        :Token(Tag::INTEGER), mNum(num){}

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
};

class Real : public Token
{
private:
    double mReal;
public:
    Real(double real)
        :Token(Tag::REAL), mReal(real) {}

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
};

class Word : public Token
{
private:
    std::string mWord;
public:
    Word(std::string s)
        :Token(Tag::WORD), mWord(s) {}

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
};

class Lexer
{
private:
    bool match(const std::string s);
public:
    Lexer() {}
    std::unique_ptr<Token> GetToken();
};

#endif
