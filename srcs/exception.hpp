#ifndef _EXCEPTION_HPP__
#define _EXCEPTION_HPP__
#include <exception>
#include <string>

class InvalidToken: public std::exception{
    private:
     std::string _result;
    public:
        InvalidToken(std::size_t index, const std::string & mesg):std::exception{}, _result{}{
            _result
            .append("Invalid Index at ")
            .append(std::to_string(index))
            .append(": ")
            .append(mesg); 
        }

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
            .append("Unmatched parenthese at index")
            .append(std::to_string(pos));
        }
        virtual const char * what() const noexcept{
            return _result.c_str();
        }
};

#endif