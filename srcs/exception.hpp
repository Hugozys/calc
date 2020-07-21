#ifndef _EXCEPTION_HPP__
#define _EXCEPTION_HPP__
#include <exception>
#include <string>
#include "tokenimpl.hpp"

class InvalidToken: public std::exception{
    private:
     std::string _result;
    public:
        InvalidToken(std::size_t index, const std::string & mesg):std::exception{}, _result{}{
            _result
            .append("Invalid token at index ")
            .append(std::to_string(index))
            .append(": ")
            .append(mesg); 
        }

        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};

class UnknownChar: public std::exception{
    std::string _result;
    public:
        UnknownChar(char c):_result{c}{}
        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};
class UnbalancedParentheses: public std::exception{
    private:
        std::string _result;

    public:
        UnbalancedParentheses(std::size_t pos):std::exception{}, _result{}{
            _result
            .append("Unmatched parenthese at index ")
            .append(std::to_string(pos));
        }
        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};


class DivideByZero: public std::exception{
    private: 
        std::string _result;
    public:
        DivideByZero(std::size_t pos):std::exception{}, _result{}{
            _result
            .append("Divide by zero expression at index ")
            .append(std::to_string(pos));
        }
        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};

class ParserError: public std::exception{
    std::string _result;
    public:
        ParserError(std::initializer_list<TokenType> expected, TokenType foundType, std::size_t pos){
            _result.append("Parser error at index ")
                   .append(std::to_string(pos))
                   .append(". ");
            _result.append("Expected tokens to be: ");
            for (auto type: expected){
                _result.append(ToString(type))
                       .append(", ");
            }
            _result.append("but found: ")
                   .append(ToString(foundType))
                   .append(".");
        }

        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};



#endif