#include <fruit/fruit.h>

#include <queue>
#include <string>
#include <vector>

#include "exception.hpp"
#include "token.hpp"
#include "validator.hpp"

class ParaValidator : public Validator {
 public:
  using Inject = ParaValidator();
  Result Validate(std::queue<PToken> tokenList) override;
  virtual ~ParaValidator() = default;

 private:
  std::vector<std::size_t> _posstack;
};

Result ParaValidator::Validate(std::queue<PToken> tokenList) {
  while (!tokenList.empty()) {
    auto token = tokenList.front();
    tokenList.pop();
    if (token.Type() == TokenType::LPARA) {
      _posstack.push_back(token.Pos());
    } else if (token.Type() == TokenType::RPARA) {
      if (_posstack.empty()) {
        return Result{false, token.Pos()};
      }
      _posstack.pop_back();
    }
  }
  if (!_posstack.empty()) {
    return Result{false, _posstack.back()};
  }
  return Result{true, std::string::npos};
}

fruit::Component<Validator> getValidatorComponent() {
  return fruit::createComponent().bind<Validator, ParaValidator>();
}
