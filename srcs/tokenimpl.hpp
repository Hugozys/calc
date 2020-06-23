#ifndef _TOKENIMPL_HPP__
#define _TOKENIMPL_HPP__

#include <iostream>

enum class TokenType{
    LPARA,
    RPARA,
    END,
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EXP,
    WS
};

bool IsValueType(TokenType type);
const char * ToString(TokenType type);
std::ostream & operator<< (std::ostream & os, TokenType type);

class Token{
    public:
        TokenType Type() const;
        const char * Value() const;
        virtual ~Token(){}
    protected:
        Token(TokenType type, const char * value):_type{type},_value(value){
        }
    private:
        TokenType _type;
        std::string _value;
};

std::ostream & operator<< (std::ostream & os, const Token & token);

class OpToken final: public Token{
    public:
        OpToken(TokenType type):Token{type, ""}{
    }
        virtual ~OpToken(){}
};


class ValToken final: public Token{
    public:
        ValToken(TokenType type, const char * value):Token{type, value}{

        }
        virtual ~ValToken(){}
};

#endif