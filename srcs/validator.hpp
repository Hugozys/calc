#ifndef SRCS_VALIDATOR_HPP_
#define SRCS_VALIDATOR_HPP_
#include <queue>

#include "token.hpp"

struct Result {
  bool IsSuccess;
  std::size_t Position;
};

class Validator {
 public:
  virtual Result Validate(std::queue<PToken> tokenList) = 0;
  virtual ~Validator() = default;
};

fruit::Component<Validator> getValidatorComponent();

#endif  //  SRCS_VALIDATOR_HPP_
