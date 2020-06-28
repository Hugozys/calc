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
        class ParserInternal;
        class ExpFactory;    
};

class Parser::ParserInternal{
    public:
        TokenType CurrentType();
        PToken Current();
        bool Eat(TokenType type);
        std::shared_ptr<Exp> ParseS();
        std::shared_ptr<Exp> ParseE();
        std::shared_ptr<Exp> ParseT();
        std::shared_ptr<Exp> ParseF();
        std::shared_ptr<Exp> ParseN();
        std::shared_ptr<Exp> ParseK();
    private:
        std::queue<PToken> _tokenQueue;

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
    public:
        std::size_t Pos() const{
            return _pos;
        }
        virtual void Accept(Visitor & visitor) const = 0;
        Exp(std::size_t pos):_pos{pos}{}
    private:
        std::size_t _pos;
};

class Parser::OpExp: public Parser::Exp{
    public:
        class ConstIterator;
        ConstIterator CBegin() const;
        ConstIterator CEnd() const;
    protected:
        OpExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):Exp{pos}, _children{list}{}
        OpExp() = default;
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
    public:
        PlusExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):OpExp{pos, list}{}
        virtual void Accept(Visitor & visitor) const;
};

class Parser::MinusExp: public Parser::OpExp{
    public:
        MinusExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):OpExp{pos, list}{}
        virtual void Accept(Visitor & visitor) const;
};

class Parser::MulExp: public Parser::OpExp{
    public:
        MulExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):OpExp{pos, list}{}
        virtual void Accept(Visitor & visitor) const;
};

class Parser::DivExp: public Parser::OpExp{
    public:
        DivExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):OpExp{pos, list}{}
        virtual void Accept(Visitor & visitor) const;
};

class Parser::ExpoExp: public Parser::OpExp{
    public:
        ExpoExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list):OpExp{pos, list}{}
        virtual void Accept(Visitor & visitor) const;
};


class Parser::IntExp: public Parser::Exp{
    public:
        IntExp(std::size_t pos, const std::string & intstr):Exp{pos},_value{intstr}{}
        virtual void Accept(Visitor & visitor) const;
    
    private:
        std::string _value;
};


class Parser::FloatExp: public Parser::Exp{
    public:
        FloatExp(std::size_t pos, const std::string & floatstr):Exp{pos},_value{floatstr}{}
    virtual void Accept(Visitor & visitor) const;
    
    private:
        std::string _value;
};

#endif