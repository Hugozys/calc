#include <cassert>
#include <vector>
#include <unordered_set>
#include "dfa.hpp"
#include "token.hpp"

std::unique_ptr<Dfa> Dfa::BuildDfa(){
    Builder b{};
    return b.AddStates().LinkStates().SetAcceptStates().SetStartState().Build();
}


void Dfa::Reset(){
    _currentState = _startState;
    _passedAcceptState = nullptr;
    _passedAcceptStateStr.clear();
    _buffer.clear();
}

bool Dfa::HasAcceptState() const{
    return _passedAcceptState != nullptr;
}

bool Dfa::IsCurrentAccept() const{
    return _acceptStates.count(_currentState);
}

PToken Dfa::Accept(int pos){
    assert(_passedAcceptState != nullptr && "The Dfa simulation never transitted to an accept state.");
    auto token = _passedAcceptState->Execute(_passedAcceptStateStr.c_str(), pos);
    Reset();
    return token;
}

bool Dfa::Move(char c){
    auto transitTable = _currentState->Table();
    if (! transitTable.count(c)){
        return false;
    }
     _buffer.push_back(c);
    _currentState = transitTable.at(c);
    if (_acceptStates.count(_currentState)){
        _passedAcceptState = _currentState;
        _passedAcceptStateStr = _buffer;
    }
    return true;
}



Dfa::Builder & Dfa::Builder::AddStates(){
    _states[1] = std::make_unique<State>(nullptr);
    _states[2] = std::make_unique<State>(ValueTokenAction);
    _states[3] = std::make_unique<State>(SpaceTokenAction);
    _states[4] = std::make_unique<State>(OpTokenAction);
    _states[5] = std::make_unique<State>(nullptr);
   return *this;
}

Dfa::Builder & Dfa::Builder::LinkStates(){
    std::vector<char> digitSet{'0', '1', '2', '3','4','5','6','7','8','9'};
    std::vector<char> opSet{'+','-','x','/', '(', ')', '^', '\n'};
    std::vector<char> spaceSet{' ', '\t'};
    for (auto c : digitSet){
        _states[1]->_table[c] = _states[2].get();
        _states[2]->_table[c] = _states[2].get();
        _states[5]->_table[c] = _states[2].get();
    }
    for (auto c : spaceSet){
        _states[1]->_table[c] = _states[3].get();
        _states[3]->_table[c] = _states[3].get();
    }
    for (auto c: opSet){
        _states[1]->_table[c] = _states[4].get();
    }
    _states[2]->_table['.'] = _states[5].get();
    _states[5]->_table['.'] = _states[2].get();
    return *this;
}

Dfa::Builder & Dfa::Builder::SetAcceptStates(){
    _acceptStates = 
    {
        _states[2].get(), 
        _states[3].get(), 
        _states[4].get()
    };
    return *this;
}

Dfa::Builder & Dfa::Builder::SetStartState(){
    _startState = _states[1].get();
    return *this;
}

void Dfa::Builder::Reset(){
    _startState = nullptr;    
    _acceptStates.clear();
    _states.clear();
}

std::unique_ptr<Dfa> Dfa::Builder::Build(){
    std::unique_ptr<Dfa> result{new Dfa{}};
    result->_acceptStates = std::move(_acceptStates);
    result->_states = std::move(_states);
    result->_startState = _startState;
    Reset();
    return result;
}