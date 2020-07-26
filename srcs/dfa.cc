#include <cassert>

#include "dfa.hpp"
#include "token.hpp"

Dfa::State Dfa::AddState(Dfa::Action action, bool accepting){
    State s = _states.size();
    _states.push_back({{}, action, accepting});
    _states[s].edges.fill(InvalidState);
    return s;
}

void Dfa::LinkStates(Dfa::State from, char input, Dfa::State to){
    assert(from < _states.size());
    assert(to < _states.size());
    _states[from].edges[(unsigned char) input] = to;
}
