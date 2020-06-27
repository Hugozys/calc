#include "parser.hpp"
#include "visitor.hpp"


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

