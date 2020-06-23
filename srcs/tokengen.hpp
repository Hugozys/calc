#ifndef _TOKENGEN__HPP_
#define _TOKENGEN__HPP_
#include <exception>
#include <queue>
#include <string>
#include "dfa.hpp"
#include "exception.hpp"
#include "token.hpp"

class TokenGen{
    public:
        TokenGen():_dfa(Dfa::BuildDfa()){}
        std::queue<PToken> Tokenize(const std::string & input);
        void Reset();
    private:
        std::unique_ptr<Dfa> _dfa;
        std::size_t _head{0};
        std::size_t _tail{0};
        std::size_t _lastAcceptTail{std::string::npos};

        PToken _acceptOrThrow(const std::string & input){
            if (_dfa->HasAcceptState()){
                _head = _lastAcceptTail + 1;
                _tail = _head;
                _lastAcceptTail = std::string::npos;
                return _dfa->Accept();
            }
            // No accept state means an error.
            // 012345678 9
            auto len = _tail == input.size() ? (_tail - _head ) : (_tail - _head + 1);
            std::string error{};
            error
            .append("invalid token at index ")
            .append(std::to_string(_head))
            .append(": ")
            .append(input.substr(_head, len));
            throw InvalidToken(error);
        }
};

#endif