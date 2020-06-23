#include <algorithm>
#include <string>
#include "token.hpp"

std::ostream & operator<< (std::ostream & os, const PToken & ptoken){
   os << *(ptoken._token);
   return os;
}

PToken ValueTokenAction(const char * payload){
    std::string pl {payload};
    PTokenFactory factory{};
    if (std::any_of(pl.begin(), pl.end(), [](char c){return c == '.';})){
        return factory.CreatePTokenWithTypeAndValue(TokenType::FLOAT, payload);
    }
    return factory.CreatePTokenWithTypeAndValue(TokenType::INT, payload);
}

PToken OpTokenAction(const char * payload){
    std::string pl {payload};
    PTokenFactory factory{};
    auto c = pl[0];
    TokenType type;
    switch (c){
        case '+':
            type = TokenType::PLUS;
            break;
        case '-':
            type = TokenType::MINUS;
            break;
        case 'x':
            type = TokenType::MUL;
            break;
        case '/':
            type = TokenType::DIV;
            break;
        case '^':
            type = TokenType::EXP;
            break;
        case '(':
            type = TokenType::LPARA;
            break;
        case ')':
            type = TokenType::RPARA;
            break;
        case '\n':
            type = TokenType::END;
            break;
    }
    return factory.CreatePTokenWithType(type);
}

PToken SpaceTokenAction(const char *){
    PTokenFactory factory{};
    return factory.CreatePTokenWithType(TokenType::WS);
}
