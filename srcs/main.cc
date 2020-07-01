#include <exception>
#include <iostream>
#include "exception.hpp"
#include "tokengen.hpp"
#include "token.hpp"
#include "parser.hpp"

int main(void){
    TokenGen tokenGenerator{};
    Parser parser{};
    while(1){
        std::string input{"-(1 + 2)^4 x 3 - 5 + 6.7"};
        //std::getline(std::cin, input);
        input.push_back('\n');
        std::queue<PToken> list{};
        try{
            parser.Parse(tokenGenerator.Tokenize(input));
            break;
        }
        catch(const InvalidToken & e){
            std::cout<<e.what()<<'\n';
            continue;
        }
    }
    return 0;
}