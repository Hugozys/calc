#ifndef _PARAVALIDATOR__HPP_
#define _PARAVALIDATOR__HPP_
#include <vector>
#include "exception.hpp"
#include "token.hpp"

class ParaValidator{
    public:
    ParaValidator() = default;
    void Validate(const PToken & ptoken, std::vector<std::size_t> & posStack){
        if ( ptoken.Type() == TokenType::LPARA){
            posStack.push_back(ptoken.Pos());
        }
        else if ( ptoken.Type() == TokenType::RPARA){
            if (posStack.empty()){
                throw UnbalancedParentheses{ptoken.Pos()};
            }
            posStack.pop_back();
        }
    }
    void Validate(const std::vector<std::size_t> & posStack){
        if (!posStack.empty()){
            throw UnbalancedParentheses{posStack.back()};
        }
    }
};

#endif