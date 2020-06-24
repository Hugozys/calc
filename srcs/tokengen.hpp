#ifndef _TOKENGEN__HPP_
#define _TOKENGEN__HPP_
#include <exception>
#include <queue>
#include <string>
#include <vector>
#include "dfa.hpp"
#include "exception.hpp"
#include "paraValidator.hpp"
#include "token.hpp"

class TokenGen{
    public:
        TokenGen():_dfa(Dfa::BuildDfa()){}
        std::queue<PToken> Tokenize(const std::string & input);
        void Reset();
    private:
        std::unique_ptr<Dfa> _dfa;
        ParaValidator _pvalidator;
        std::vector<std::size_t> _pstack;
        int _paraNum{0};
        std::size_t _head{0};
        std::size_t _tail{0};
        std::size_t _lastAcceptTail{std::string::npos};

        PToken _acceptOrThrow(const std::string & input){
            if (_dfa->HasAcceptState()){
                auto ptoken{_dfa->Accept(_head)};
                _head = _lastAcceptTail + 1;
                _tail = _head;
                _lastAcceptTail = std::string::npos;
                _pvalidator.Validate(ptoken, _pstack);
                return ptoken;
            }
            // No accept state means an error.
            auto len = _tail == input.size() ? (_tail - _head ) : (_tail - _head + 1);
            throw InvalidToken(_head, input.substr(_head, len));
        }
};

#endif