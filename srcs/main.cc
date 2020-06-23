#include <exception>
#include <iostream>
#include "exception.hpp"
#include "tokengen.hpp"
#include "token.hpp"

int main(void){
    TokenGen tokenGenerator{};
    while(1){
        std::string input{};
        std::getline(std::cin, input);
        input.push_back('\n');
        std::queue<PToken> list{};
        try{
            list = tokenGenerator.Tokenize(input);
        }
        catch(const InvalidToken & e){
            std::cout<<e.what()<<'\n';
            continue;
        }
        while (!list.empty()){
            std::cout<<list.front()<<" ";
            list.pop();
        }
    }
    return 0;
}