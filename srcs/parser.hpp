#ifndef _PARSER_HPP__
#define _PARSER_HPP__
#include <queue>
#include <vector>
#include <memory>
#include "token.hpp"
#include "visitor.hpp"

class Parser{
    public:
        class SyntaxTree;
        SyntaxTree Parse(std::queue<PToken> & tokenPool);
    private:
        class Exp;
        class OpExp;
        class PlusExp;
        class MinusExp;
        class MulExp;
        class DivExp;
        class ExpoExp;
        class IntExp;
        class FloatExp;    
};

class Parser::SyntaxTree{
    public:
    const Exp & Root() const{
        return *_rootExp;
    }
    std::shared_ptr<Exp>  _rootExp;
    SyntaxTree()= default;
};

class Parser::Exp{
    virtual void Accept(Visitor & visitor) const = 0;
};

class Parser::OpExp: public Parser::Exp{
    public:
        class ConstIterator;
        ConstIterator CBegin() const;
        ConstIterator CEnd() const;
    protected:
        OpExp(std::initializer_list<std::shared_ptr<Exp>> list):_children{list}{}
    private:
        using ExpIt = std::vector<std::shared_ptr<Exp>>::iterator;
        using ConstExpIt = std::vector<std::shared_ptr<Exp>>::const_iterator;
        std::vector<std::shared_ptr<Exp>> _children;
};

 class Parser::OpExp::ConstIterator{
        public:
            friend Parser::OpExp::ConstIterator Parser::OpExp::CBegin() const;
            friend Parser::OpExp::ConstIterator Parser::OpExp::CEnd() const;
            ConstIterator & operator ++(){
                ++_expIt;
                return *this;
            }
        private:
            ConstIterator(ConstExpIt it):_expIt{it}{};
            ConstExpIt _expIt;
};

class Parser::PlusExp: public Parser::OpExp{
    virtual void Accept(Visitor & visitor) const;
};

class Parser::MinusExp: public Parser::OpExp{
    virtual void Accept(Visitor & visitor) const;
};

class Parser::MulExp: public Parser::OpExp{
    virtual void Accept(Visitor & visitor) const;
};

class Parser::DivExp: public Parser::OpExp{
    virtual void Accept(Visitor & visitor) const;
};

class Parser::ExpoExp: public Parser::OpExp{
    virtual void Accept(Visitor & visitor) const;
};


class Parser::IntExp: public Parser::Exp{
    virtual void Accept(Visitor & visitor) const;
};


class Parser::FloatExp: public Parser::Exp{
    virtual void Accept(Visitor & visitor) const;
};


#endif