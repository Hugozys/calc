#ifndef SRCS_TOKENGEN_HPP_
#define SRCS_TOKENGEN_HPP_
#include <fruit/fruit.h>

#include <exception>
#include <queue>
#include <string>
#include <vector>

#include "dfa.hpp"
#include "token.hpp"
#include "validator.hpp"

class TokenGen {
 public:
  virtual std::queue<PToken> Tokenize(const std::string& input) = 0;
  virtual ~TokenGen() = default;
};

fruit::Component<TokenGen> getTokenGenComponent();
#endif  //  SRCS_TOKENGEN_HPP_
