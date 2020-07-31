#include <queue>
#include <string>

#include "calcdfafactory.hpp"
#include "dfa.hpp"
#include "dfafactory.hpp"
#include "exception.hpp"
#include "token.hpp"
#include "tokengen.hpp"
#include "validator.hpp"
using dfa::Dfa;
using dfa::DfaFactory;
using dfa::getCalcDfaFactoryComponent;

class TokenGenImpl : public TokenGen {
 public:
  using Inject = TokenGenImpl(DfaFactory &, Validator &);
  TokenGenImpl(DfaFactory &factory, Validator &validator)
      : _dfafactory{factory},
        _validator{validator},
        _dfa{_dfafactory.CreateDfa()} {}
  std::queue<PToken> Tokenize(const std::string &input) override;
  virtual ~TokenGenImpl() = default;

 private:
  DfaFactory &_dfafactory;
  Validator &_validator;
  Dfa _dfa;
  PToken _acceptOrThrow(const std::string &input, std::size_t &head,
                        std::size_t &tail, Dfa::State &latestAcceptState,
                        std::size_t &latestAcceptTail);
};

std::queue<PToken> TokenGenImpl::Tokenize(const std::string &input) {
  Dfa::State _currentState{_dfa.GetStartState()};
  std::size_t _head{0};
  std::size_t _tail{0};
  std::size_t _latestAcceptTail{std::string::npos};
  Dfa::State _latestAcceptState{Dfa::InvalidState};
  std::queue<PToken> tokenList{};
  while (_head != input.size()) {
    if (_tail == input.size()) {
      tokenList.push(_acceptOrThrow(input, _head, _tail, _latestAcceptState,
                                    _latestAcceptTail));
    } else {
      auto tempState = _dfa.GetNextState(_currentState, input[_tail]);
      if (tempState != Dfa::InvalidState) {
        _currentState = tempState;
        if (_dfa.IsAccept(_currentState)) {
          _latestAcceptTail = _tail;
          _latestAcceptState = _currentState;
        }
        ++_tail;
      } else {
        tokenList.push(_acceptOrThrow(input, _head, _tail, _latestAcceptState,
                                      _latestAcceptTail));
        _currentState = _dfa.GetStartState();
      }
    }
  }
  _validator.Validate(tokenList);
  return tokenList;
}

PToken TokenGenImpl::_acceptOrThrow(const std::string &input, std::size_t &head,
                                    std::size_t &tail,
                                    Dfa::State &latestAcceptState,
                                    std::size_t &latestAcceptTail) {
  if (latestAcceptState != Dfa::InvalidState) {
    auto action = _dfa.GetStateAction(latestAcceptState);
    auto ptoken =
        action(input.substr(head, latestAcceptTail - head + 1).c_str(), head);
    head = latestAcceptTail + 1;
    tail = head;
    latestAcceptTail = std::string::npos;
    latestAcceptState = Dfa::InvalidState;
    return ptoken;
  }
  // No accept state means an error.
  auto len = tail == input.size() ? (tail - head) : (tail - head + 1);
  throw InvalidToken(head, input.substr(head, len));
}

fruit::Component<TokenGen> getTokenGenComponent() {
  return fruit::createComponent()
      .bind<TokenGen, TokenGenImpl>()
      .install(getCalcDfaFactoryComponent)
      .install(getValidatorComponent);
}
