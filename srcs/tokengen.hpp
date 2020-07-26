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
        TokenGen();
        std::queue<PToken> Tokenize(const std::string & input);
    private:
        std::unique_ptr<Dfa> _dfa;
};

#endif
