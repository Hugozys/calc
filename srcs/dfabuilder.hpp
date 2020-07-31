#ifndef SRCS_DFABUILDER_HPP_
#define SRCS_DFABUILDER_HPP_
#include <unordered_map>
#include <unordered_set>

#include "dfa.hpp"
namespace dfa {
class Dfa::DfaBuilder {
 public:
  DfaBuilder() = default;
  DfaBuilder& WithState(State& state, Action action = nullptr);
  DfaBuilder& WithEdge(State from, State to, char transitch);
  DfaBuilder& WithStartState(State start);
  ~DfaBuilder() = default;
  Dfa Build() { return Dfa{_startState, _states, _transitTable, _actionMap}; }
  void Clean() {
    _startState = Dfa::InvalidState;
    _states.clear();
    _transitTable.clear();
    _actionMap.clear();
  }

 private:
  State _startState;
  std::unordered_set<State> _states;
  std::unordered_map<State, std::unordered_map<char, State>> _transitTable;
  std::unordered_map<State, Action> _actionMap;
};
}  // namespace dfa

#endif  //  SRCS_DFABUILDER_HPP_
