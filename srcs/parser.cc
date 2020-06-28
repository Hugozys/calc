#include <cassert>
#include <memory>
#include "exception.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "visitor.hpp"

PToken Parser::ParserInternal::Current(){
     while (_tokenQueue.front().Type() == TokenType::WS){
        _tokenQueue.pop();
    }
    return _tokenQueue.front();
}

TokenType Parser::ParserInternal::CurrentType() {
   return Current().Type();
} 

bool Parser::ParserInternal::Eat(TokenType type){
    if (type != CurrentType()){
        return false;
    }
    _tokenQueue.pop();
    return true;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseS(){
    auto e = ParseE();
    if (!Eat(TokenType::END)){
        throw std::exception();
    }
    return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseE(){
    auto e = ParseT();
    while (true){
        auto lookAhead = CurrentType();
        auto currentToken = Current();
        if (!Eat(TokenType::PLUS) && !Eat(TokenType::MINUS)){
            break;
        }
        else if (lookAhead == TokenType::PLUS){
            e = std::shared_ptr<PlusExp>{new PlusExp{currentToken.Pos(), {e, ParseT()}}};
        }
        else if (lookAhead == TokenType::MINUS){
            e = std::shared_ptr<MinusExp>{new MinusExp{currentToken.Pos(), {e, ParseT()}}};
        }
        else{
            // TODO: throw exception
            throw std::exception{};
        }
    }
    return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseT(){
    auto e = ParseF();
    while (true){
        auto lookAhead = CurrentType();
        auto currentToken = Current();
        if (!Eat(TokenType::MUL) && !Eat(TokenType::DIV)){
            break;
        }
        else if (lookAhead == TokenType::MUL){
            e = std::shared_ptr<MulExp>{new MulExp{currentToken.Pos(), {e, ParseF()}}};
        }
        else if (lookAhead == TokenType::DIV){
            e = std::shared_ptr<DivExp>{new DivExp{currentToken.Pos(), {e, ParseF()}}};
        }
        else{
            // TODO: 
            throw std::exception{};
        }
    }
    return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseF(){
    auto e = ParseN();
    auto pos = Current().Pos();
    // refactor this
    while (Eat(TokenType::EXP)){
        e = std::shared_ptr<ExpoExp>{new ExpoExp{pos, {e, ParseN()}}};
        pos = Current().Pos();
    }
    return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseN(){
    auto currentToken = Current();
    if (Eat(TokenType::PLUS)){
         return std::shared_ptr<PlusExp>{new PlusExp{currentToken.Pos(), {ParseK()}}};
    }
    if (Eat(TokenType::MINUS)){
        return std::shared_ptr<MinusExp>{new MinusExp{currentToken.Pos(), {ParseK()}}};
    }
    return ParseK();
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseK(){
    auto currentToken = Current();
    if (Eat(TokenType::LPARA)){
        auto e = ParseE();
        if (!Eat(TokenType::RPARA)){
            throw std::exception();
        }
        return e;
    }
    if (Eat(TokenType::INT)){
        return std::make_shared<IntExp>(currentToken.Pos(), currentToken.Value());
    }
    if (Eat(TokenType::FLOAT)){
        return std::make_shared<FloatExp>(currentToken.Pos(), currentToken.Value());
    }
    throw std::exception{};
}

void Parser::PlusExp::Accept(Visitor & visitor) const{

}

void Parser::MinusExp::Accept(Visitor & visitor) const{

}


void Parser::MulExp::Accept(Visitor & visitor) const{

}

void Parser::DivExp::Accept(Visitor & visitor) const{

}

void Parser::ExpoExp::Accept(Visitor & visitor) const{

}

void Parser::IntExp::Accept(Visitor & visitor) const{

}

void Parser::FloatExp::Accept(Visitor & visitor) const{

}

Parser::OpExp::ConstIterator Parser::OpExp::CBegin() const{
    return ConstIterator{_children.cbegin()};
}

Parser::OpExp::ConstIterator Parser::OpExp::CEnd() const{
    return ConstIterator{_children.cend()};
}

