#include <queue>
#include <string>

#include "token.hpp"
#include "tokengen.hpp"

namespace {

std::unique_ptr<Dfa> BuildDfa(){
    auto dfa = std::make_unique<Dfa>();
    Dfa::State states[] = {
        /*0*/dfa->AddState(/*action=*/nullptr, /*accepting=*/false),
        /*1*/dfa->AddState(/*action=*/ValueTokenAction, /*accepting=*/true),
        /*2*/dfa->AddState(/*action=*/SpaceTokenAction, /*accepting=*/true),
        /*3*/dfa->AddState(/*action=*/OpTokenAction, /*accepting=*/true),
        /*4*/dfa->AddState(/*action=*/nullptr, /*accepting=*/false),
    };

    // link states
    static const char digitSet[] = {'0', '1', '2', '3','4','5','6','7','8','9'};
    static const char opSet[] = {'+','-','x','/', '(', ')', '^', '\n'};
    static const char spaceSet[] = {' ', '\t'};
    for (auto c : digitSet){
        dfa->LinkStates(states[0], c, states[1]);
        dfa->LinkStates(states[1], c, states[1]);
        dfa->LinkStates(states[4], c, states[1]);
    }
    for (auto c : spaceSet){
        dfa->LinkStates(states[0], c, states[2]);
        dfa->LinkStates(states[2], c, states[2]);
    }
    for (auto c: opSet){
        dfa->LinkStates(states[0], c, states[3]);
    }
    dfa->LinkStates(states[1], '.', states[4]);
    dfa->LinkStates(states[4], '.', states[1]);
    return dfa;
}

}  // namespace

TokenGen::TokenGen() : _dfa(BuildDfa()), _dfa_sim(*_dfa.get()) {}

void TokenGen::Reset(){
    _head = 0;
    _tail = 0;
    _lastAcceptTail = std::string::npos;
    _dfa_sim.Reset();
    _pstack.clear();
}

std::queue<PToken> TokenGen::Tokenize(const std::string & input){
    Reset();
    std::queue<PToken> tokenList{};
    while (_head != input.size()){
        if (_tail == input.size()){
            tokenList.push(_acceptOrThrow(input));
        }
        else{ 
            if(_dfa_sim.Move(input[_tail])){
                if (_dfa_sim.IsCurrentAccept()){
                    _lastAcceptTail = _tail;
                }
                ++_tail;
            }
            else{
                tokenList.push(_acceptOrThrow(input));
            }
        }
    }
    _pvalidator.Validate(_pstack);
    return tokenList;
}
