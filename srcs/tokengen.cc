#include <queue>
#include <string>
#include "token.hpp"
#include "tokengen.hpp"

void TokenGen::Reset(){
    _head = 0;
    _tail = 0;
    _lastAcceptTail = std::string::npos;
    _dfa->Reset();
}

std::queue<PToken> TokenGen::Tokenize(const std::string & input){
    Reset();
    std::queue<PToken> tokenList{};
    while (_head != input.size()){
        if (_tail == input.size()){
            tokenList.push(_acceptOrThrow(input));
        }
        else{ 
            if(_dfa->Move(input[_tail])){
                if (_dfa->IsCurrentAccept()){
                    _lastAcceptTail = _tail;
                }
                ++_tail;
            }
            else{
                tokenList.push(_acceptOrThrow(input));
            }
        }
    }
    return tokenList;
}
