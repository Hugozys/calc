#include <cassert>
#include <unordered_set>
#include <vector>

#include "dfa.hpp"
#include "exception.hpp"
#include "token.hpp"
using dfa::Dfa;

Dfa::Action Dfa::GetStateAction(State state) const {
  auto it = _states.find(state);
  if (it == _states.end()) {
    throw NoSuchState{};
  }
  if (!IsAccept(state)) {
    return nullptr;
  }
  return _actionMap.at(state);
}

Dfa::State Dfa::GetStartState() const { return _startState; }

bool Dfa::IsAccept(Dfa::State state) const {
  auto it = _actionMap.find(state);
  return it != _actionMap.end();
}

Dfa::State Dfa::GetNextState(Dfa::State state, char c) const {
  auto transit = _transitTable.at(state);
  auto it = transit.find(c);
  if (it == transit.end()) {
    return Dfa::InvalidState;
  }
  return it->second;
}
