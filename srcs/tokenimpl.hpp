#ifndef SRCS_TOKENIMPL_HPP_
#define SRCS_TOKENIMPL_HPP_

#include <iostream>
#include <string>
enum class TokenType {
  LPARA,
  RPARA,
  END,
  INT,
  FLOAT,
  PLUS,
  MINUS,
  MUL,
  DIV,
  EXP,
  WS,
};

bool IsValueType(TokenType type);
const char* ToString(TokenType type);
std::ostream& operator<<(std::ostream& os, TokenType type);

class Token {
 public:
  TokenType Type() const;
  const char* Value() const;
  int Pos() const;
  virtual ~Token() {}

 protected:
  Token(TokenType type, const char* value, int pos)
      : _type{type}, _value(value), _pos(pos) {}

 private:
  TokenType _type;
  std::string _value;
  int _pos;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

class OpToken final : public Token {
 public:
  OpToken(TokenType type, int pos) : Token{type, "", pos} {}
  virtual ~OpToken() {}
};

class ValToken final : public Token {
 public:
  ValToken(TokenType type, const char* value, int pos)
      : Token{type, value, pos} {}
  virtual ~ValToken() {}
};

#endif  // SRCS_TOKENIMPL_HPP_
