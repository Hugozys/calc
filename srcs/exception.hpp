#ifndef _EXCEPTION_HPP__
#define _EXCEPTION_HPP__
#include <exception>
#include <string>

class InvalidToken: public std::exception{
    private:
     std::string  _message;
    
    public:
        InvalidToken(const std::string & mesg):std::exception{}, _message{mesg}{

        }
        virtual const char * what() const noexcept{
            return _message.c_str();
        }
};

#endif