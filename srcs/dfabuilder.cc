#include <cassert>

#include "dfabuilder.hpp"

using dfa::Dfa;
Dfa::DfaBuilder &Dfa::DfaBuilder::WithState(Dfa::State &state,
                                            Dfa::Action action) {
  state = _states.size();
  _states.insert(state);
  _transitTable[state];
  if (action != nullptr) {
    _actionMap[state] = action;
  }
  return *this;
}

Dfa::DfaBuilder &Dfa::DfaBuilder::WithEdge(Dfa::State from, Dfa::State to,
                                           char transitch) {
  assert(_states.find(from) != _states.end() && "from state doesn't exist.");
  assert(_states.find(to) != _states.end() && "to state doesn't exist.");
  _transitTable[from][transitch] = to;
  return *this;
}

Dfa::DfaBuilder &Dfa::DfaBuilder::WithStartState(Dfa::State start) {
  assert(_states.find(start) != _states.end() &&
         "can't set non-existing state to start state.");
  _startState = start;
  return *this;
}
