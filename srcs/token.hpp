#ifndef SRCS_TOKEN_HPP_
#define SRCS_TOKEN_HPP_
#include <iostream>
#include <memory>

#include "tokenimpl.hpp"

class PToken {
 public:
  friend class PTokenFactory;
  TokenType Type() const { return _token->Type(); }
  const char* Value() const { return _token->Value(); }
  std::size_t Pos() const { return _token->Pos(); }
  friend std::ostream& operator<<(std::ostream& os, const PToken& ptoken);

 private:
  PToken(TokenType type, std::size_t pos)
      : _token{std::make_shared<OpToken>(type, pos)} {}
  PToken(TokenType type, const char* value, std::size_t pos)
      : _token{std::make_shared<ValToken>(type, value, pos)} {}
  std::shared_ptr<Token> _token;
};

PToken ValueTokenAction(const char* payload, std::size_t pos);
PToken OpTokenAction(const char* payload, std::size_t pos);
PToken SpaceTokenAction(const char* payload, std::size_t pos);

class PTokenFactory {
 public:
  PToken CreatePTokenWithType(TokenType type, std::size_t pos) const {
    return PToken(type, pos);
  }
  PToken CreatePTokenWithTypeAndValue(TokenType type, const char* value,
                                      std::size_t pos) const {
    return PToken(type, value, pos);
  }
};

#endif  // SRCS_TOKEN_HPP_
