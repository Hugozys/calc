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

void DfaSimulator::Reset(){
    _currentState = Dfa::StartState;
    _passedAcceptState = Dfa::InvalidState;
    _passedAcceptStateStrLen = 0;
    _buffer.clear();
}

PToken DfaSimulator::Accept(int pos){
    assert(_passedAcceptState != Dfa::InvalidState && "The Dfa simulation never transitted to an accept state.");
    std::string token_str = _buffer.substr(0, _passedAcceptStateStrLen);
    Dfa::Action action = _dfa.GetAction(_passedAcceptState);
    Reset();
    return action(token_str.c_str(), pos);
}

bool DfaSimulator::Move(char c){
    Dfa::State next = _dfa.GetLink(_currentState, c);
    if (next == Dfa::InvalidState) {
        return false;
    }
    _buffer.push_back(c);
    _currentState = next;
    if (_dfa.IsAccepting(_currentState)){
        _passedAcceptState = _currentState;
        _passedAcceptStateStrLen = _buffer.size();
    }
    return true;
}
