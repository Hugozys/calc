#include "parser.hpp"

#include <cassert>
#include <memory>

#include "exception.hpp"
#include "token.hpp"

double Parser::Parse(const std::queue<PToken>& tokenPool, bool debugMode,
                     const char* filename) {
  ParserInternal internal{tokenPool};
  SyntaxTree tree{internal.ParseS()};
  if (debugMode) {
    PrintTreeVisitor printVisitor{filename};
    tree.Root().Accept(printVisitor);
  }
  EvalVisitor evalVisitor{};
  tree.Root().Accept(evalVisitor);
  return evalVisitor.Value();
}

PToken Parser::ParserInternal::Current() {
  while (_tokenQueue.front().Type() == TokenType::WS) {
    _tokenQueue.pop();
  }
  return _tokenQueue.front();
}

TokenType Parser::ParserInternal::CurrentType() { return Current().Type(); }

bool Parser::ParserInternal::Eat(TokenType type) {
  if (type != CurrentType()) {
    return false;
  }
  _tokenQueue.pop();
  return true;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseS() {
  auto e = ParseE();
  if (!Eat(TokenType::END)) {
    auto currentToken = Current();
    throw ParserError{
        {TokenType::END}, currentToken.Type(), currentToken.Pos()};
  }
  return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseE() {
  auto e = ParseT();
  while (true) {
    auto lookAhead = CurrentType();
    auto currentToken = Current();
    if (!Eat(TokenType::PLUS) && !Eat(TokenType::MINUS)) {
      break;
    } else if (lookAhead == TokenType::PLUS) {
      auto lhs = e;
      auto rhs = ParseT();
      e = std::shared_ptr<PlusExp>{new PlusExp{currentToken.Pos(), {lhs, rhs}}};
      rhs->SetParent(e.get());
      lhs->SetParent(e.get());
    } else if (lookAhead == TokenType::MINUS) {
      auto lhs = e;
      auto rhs = ParseT();
      e = std::shared_ptr<MinusExp>{new MinusExp{currentToken.Pos(), {e, rhs}}};
      rhs->SetParent(e.get());
      lhs->SetParent(e.get());
    }
  }
  return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseT() {
  auto e = ParseF();
  while (true) {
    auto lookAhead = CurrentType();
    auto currentToken = Current();
    if (!Eat(TokenType::MUL) && !Eat(TokenType::DIV)) {
      break;
    } else if (lookAhead == TokenType::MUL) {
      auto lhs = e;
      auto rhs = ParseF();
      e = std::shared_ptr<MulExp>{new MulExp{currentToken.Pos(), {lhs, rhs}}};
      lhs->SetParent(e.get());
      rhs->SetParent(e.get());
    } else if (lookAhead == TokenType::DIV) {
      auto lhs = e;
      auto rhs = ParseF();
      e = std::shared_ptr<DivExp>{new DivExp{currentToken.Pos(), {lhs, rhs}}};
      lhs->SetParent(e.get());
      rhs->SetParent(e.get());
    }
  }
  return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseF() {
  auto e = ParseN();
  while (true) {
    auto currentToken = Current();
    if (!Eat(TokenType::EXP)) {
      break;
    }
    auto lhs = e;
    auto rhs = ParseN();
    e = std::shared_ptr<ExpoExp>{new ExpoExp{currentToken.Pos(), {lhs, rhs}}};
    lhs->SetParent(e.get());
    rhs->SetParent(e.get());
  }
  return e;
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseN() {
  auto currentToken = Current();
  if (Eat(TokenType::PLUS)) {
    auto lhs = ParseN();
    auto e = std::shared_ptr<PlusExp>{new PlusExp{currentToken.Pos(), {lhs}}};
    lhs->SetParent(e.get());
    return e;
  }
  if (Eat(TokenType::MINUS)) {
    auto lhs = ParseN();
    auto e = std::shared_ptr<MinusExp>{new MinusExp{currentToken.Pos(), {lhs}}};
    lhs->SetParent(e.get());
    return e;
  }
  return ParseK();
}

std::shared_ptr<Parser::Exp> Parser::ParserInternal::ParseK() {
  auto currentToken = Current();
  if (Eat(TokenType::LPARA)) {
    auto e = ParseE();
    if (!Eat(TokenType::RPARA)) {
      auto currToken = Current();
      throw ParserError{{TokenType::RPARA}, currToken.Type(), currToken.Pos()};
    }
    return e;
  }
  if (Eat(TokenType::INT)) {
    return std::make_shared<IntExp>(currentToken.Pos(), currentToken.Value());
  }
  if (Eat(TokenType::FLOAT)) {
    return std::make_shared<FloatExp>(currentToken.Pos(), currentToken.Value());
  }
  throw ParserError{{TokenType::LPARA, TokenType::INT, TokenType::FLOAT},
                    currentToken.Type(),
                    currentToken.Pos()};
}

void Parser::PlusExp::Accept(Visitor& visitor) const {
  visitor.VisitPlusExp(*this);
}

void Parser::MinusExp::Accept(Visitor& visitor) const {
  visitor.VisitMinusExp(*this);
}

void Parser::MulExp::Accept(Visitor& visitor) const {
  visitor.VisitMulExp(*this);
}

void Parser::DivExp::Accept(Visitor& visitor) const {
  visitor.VisitDivExp(*this);
}

void Parser::ExpoExp::Accept(Visitor& visitor) const {
  visitor.VisitExpoExp(*this);
}

void Parser::IntExp::Accept(Visitor& visitor) const {
  visitor.VisitIntExp(*this);
}

void Parser::FloatExp::Accept(Visitor& visitor) const {
  visitor.VisitFloatExp(*this);
}

Parser::OpExp::ConstIterator Parser::OpExp::CBegin() const {
  return ConstIterator{_children.cbegin()};
}

Parser::OpExp::ConstIterator Parser::OpExp::CEnd() const {
  return ConstIterator{_children.cend()};
}
