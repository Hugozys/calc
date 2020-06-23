#include "tokenimpl.hpp"

TokenType Token::Type() const{
    return _type;
}

const char * Token::Value() const{
    return _value.c_str();
}

const char * ToString(TokenType type){
    switch(type){
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::MUL:
            return "MUL";
        case TokenType::DIV:
            return "DIV";
        case TokenType::EXP:
            return "EXP";
        case TokenType::LPARA:
            return "LPARA";
        case TokenType::RPARA:
            return "RPARA";
        case TokenType::INT:
            return "INT";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::END:
            return "END";
        case TokenType::WS:
            return "WS";
        default:
            throw "unimplemented";
    }

}

std::ostream & operator<<(std::ostream & os, TokenType type){
    os << ToString(type);
    return os;   
}

bool IsValueType(TokenType type){
    switch(type){
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::EXP:
        case TokenType::LPARA:
        case TokenType::RPARA:
        case TokenType::END:
        case TokenType::WS:
            return false;
        case TokenType::INT:
        case TokenType::FLOAT:
            return true;
        default:
            return false;
    }
}

std::ostream & operator<< (std::ostream & os, const Token & token){
    os << token.Type();
    if (IsValueType(token.Type())){
        os << "( "<<token.Value()<< " )";
    }
    return os;
}