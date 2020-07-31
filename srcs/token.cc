#include "token.hpp"

#include <algorithm>
#include <string>

#include "exception.hpp"

std::ostream &operator<<(std::ostream &os, const PToken &ptoken) {
  os << *(ptoken._token);
  return os;
}

PToken ValueTokenAction(const char *payload, std::size_t pos) {
  std::string pl{payload};
  PTokenFactory factory{};
  if (std::any_of(pl.begin(), pl.end(), [](char c) { return c == '.'; })) {
    return factory.CreatePTokenWithTypeAndValue(TokenType::FLOAT, payload, pos);
  }
  return factory.CreatePTokenWithTypeAndValue(TokenType::INT, payload, pos);
}

PToken OpTokenAction(const char *payload, std::size_t pos) {
  std::string pl{payload};
  PTokenFactory factory{};
  auto c = pl[0];
  TokenType type;
  switch (c) {
    case '+':
      type = TokenType::PLUS;
      break;
    case '-':
      type = TokenType::MINUS;
      break;
    case 'x':
      type = TokenType::MUL;
      break;
    case '/':
      type = TokenType::DIV;
      break;
    case '^':
      type = TokenType::EXP;
      break;
    case '(':
      type = TokenType::LPARA;
      break;
    case ')':
      type = TokenType::RPARA;
      break;
    case '\n':
      type = TokenType::END;
      break;
    default:
      throw UnknownChar{c};
  }
  return factory.CreatePTokenWithType(type, pos);
}

PToken SpaceTokenAction(const char *, std::size_t pos) {
  PTokenFactory factory{};
  return factory.CreatePTokenWithType(TokenType::WS, pos);
}
