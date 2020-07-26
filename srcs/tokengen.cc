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

TokenGen::TokenGen() : _dfa(BuildDfa()) {}

std::queue<PToken> TokenGen::Tokenize(const std::string & input){
    std::queue<PToken> tokenList{};
    ParaValidator pvalidator;
    std::vector<std::size_t> pstack;
    Dfa::State current = _dfa->Begin(), last_accept = _dfa->End();
    size_t head = 0, tail = 0, token_len = 0;

    while (head != input.size()){
        if (tail < input.size()){
            current = _dfa->GetLink(current, input[tail]);
            if (current != _dfa->End()){
                ++tail;
                if (_dfa->IsAccepting(current)){
                    last_accept = current;
                    token_len = tail - head;
                }
                continue;
            }
        }
        std::string token_str = input.substr(head, token_len);
        if (last_accept == _dfa->End()){
            // No accept state means an error.
            throw InvalidToken(head, token_str);
        }
        auto action = _dfa->GetAction(last_accept);
        PToken ptoken = action(token_str.c_str(), head);
        tail = head += token_len;
        token_len = 0;
        current = _dfa->Begin();
        last_accept = _dfa->End();
        pvalidator.Validate(ptoken, pstack);
        tokenList.push(ptoken);
    }
    pvalidator.Validate(pstack);
    return tokenList;
}
