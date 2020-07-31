#ifndef SRCS_DFA_HPP_
#define SRCS_DFA_HPP_
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "token.hpp"
namespace dfa {
class Dfa {
 public:
  using State = std::size_t;
  using Action = std::function<PToken(const char*, std::size_t)>;
  static constexpr std::size_t InvalidState = -1;
  class DfaBuilder;
  Action GetStateAction(State state) const;
  State GetStartState() const;
  bool IsAccept(State state) const;
  State GetNextState(State state, char c) const;
  Dfa() = delete;
  Dfa(Dfa&& rhs) = default;
  Dfa& operator=(Dfa&& rhs) = default;

 private:
  Dfa(State startState, std::unordered_set<State>& states,
      std::unordered_map<State, std::unordered_map<char, State>>& transitTable,
      std::unordered_map<State, Action>& actionMap)
      : _startState{startState},
        _states{std::move(states)},
        _transitTable{std::move(transitTable)},
        _actionMap{std::move(actionMap)} {}

  State _startState{};
  std::unordered_set<State> _states;
  std::unordered_map<State, std::unordered_map<char, State>> _transitTable;
  std::unordered_map<State, Action> _actionMap;
};
}  // namespace dfa
#endif  // SRCS_DFA_HPP_
