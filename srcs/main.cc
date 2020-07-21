#include <exception>
#include <iostream>
#include "exception.hpp"
#include "tokengen.hpp"
#include "token.hpp"
#include "parser.hpp"

int main(void){
    TokenGen tokenGenerator{};
    Parser parser{};
    std::cout<<"calc > ";
    while(1){
        std::string input{};
        std::getline(std::cin, input);
        if (input == "quit"){
            return EXIT_SUCCESS;
        }
        input.push_back('\n');
        std::queue<PToken> list{};
        try{
            auto result = parser.Parse(tokenGenerator.Tokenize(input));
            std::cout<<result<<'\n';
        }
        catch(const std::exception & e){
            std::cout<<e.what()<<'\n';
        }
        std::cout<<"calc > ";
    }
    return 0;
}