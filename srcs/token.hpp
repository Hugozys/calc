#ifndef _TOKEN_HPP__
#define _TOKEN_HPP__
#include <iostream>
#include <memory>
#include "tokenimpl.hpp"

class PToken{
    public:
        friend class PTokenFactory;
        TokenType Type() const{
            return _token->Type();
        }
        const char * Value() const{
            return _token->Value();
        }
        friend std::ostream& operator<<(std::ostream & os, const PToken & ptoken);
    private:
        PToken(TokenType type):_token{std::make_shared<OpToken>(type)}{}
        PToken(TokenType type, const char * value):_token{std::make_shared<ValToken>(type, value)}{}
        std::shared_ptr<Token> _token;
};

PToken ValueTokenAction(const char * payload);
PToken OpTokenAction(const char * payload);
PToken SpaceTokenAction(const char * payload);

class PTokenFactory{
    public:
        PToken CreatePTokenWithType(TokenType type) const{
            return PToken(type);
        }
        PToken CreatePTokenWithTypeAndValue(TokenType type, const char * value) const{
            return PToken(type, value);
        }
};

#endif