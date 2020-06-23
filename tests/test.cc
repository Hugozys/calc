#include "tokengen.hpp"
#include "token.hpp"
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>

std::string print(std::queue<PToken> & list){
    std::stringstream ss{};
    while(!list.empty()){
        ss << list.front() << " ";
        list.pop();
    }
    return ss.str();
}
int main(void){
    std::unordered_map<std::string, std::string> test_suite{
        {"1", "INT( 1 )"},
        {"2", "INT( 2 )"},
        {"0", "INT( 0 )"},
        {"123", "INT( 123 )"},
        {"09", "INT( 09 )"},
        {"1.2", "FLOAT( 1.2 )"},
        {"0.01", "FLOAT( 0.01 )"},
        {"3.141592653", "FLOAT( 3.141592653 )"},
        {"+", "PLUS"},
        {"-", "MINUS"},
        {"x", "MUL"},
        {"/", "DIV"},
        {"^", "EXP"},
        {" ", "WS"},
        {"\t", "WS"},
        {"\n", "END"},
        {"(", "LPARA"},
        {")", "RPARA"},
        {"1+2", "INT( 1 ) PLUS INT( 2 )"},
        {"1x2", "INT( 1 ) MUL INT( 2 )"},
        {"1-2", "INT( 1 ) MINUS INT( 2 )"},
        {"-1x3", "MINUS INT( 1 ) MUL INT( 3 )"},
        {"1/2", "INT( 1 ) DIV INT( 2 )"},
        {"1^2", "INT( 1 ) EXP INT( 2 )"},
        {"1+3/4x5-7^8/9x3.14159\n", "INT( 1 ) PLUS INT( 3 ) DIV INT( 4 ) MUL INT( 5 ) MINUS INT( 7 ) EXP INT( 8 ) DIV INT( 9 ) MUL FLOAT( 3.14159 ) END"},
        {"1+3 x   4 -   5 \n", "INT( 1 ) PLUS INT( 3 ) MUL WS INT( 4 ) WS MINUS WS INT( 5 ) WS END"},
        {"(( 3.2 + 4.97)x27 / -9 )^(1/5)\n", "LPARA LPARA WS FLOAT( 3.2 ) WS PLUS WS FLOAT( 4.97 ) RPARA MUL INT( 27 ) WS DIV WS MINUS INT( 9 ) WS RPARA EXP LPARA INT( 1 ) DIV INT( 5 ) RPARA END"}
    };
    TokenGen sut{};
    int i = 1;
    for(const auto & pair : test_suite){
        sut.Reset();
        auto tokenList{sut.Tokenize(pair.first)};
        auto result{print(tokenList)};
        if(! result.compare(pair.first)){
            std::cout << "input: " << pair.first;
            std::cout << "expected: "<<pair.second<<"\n";
            std::cout << "but  get: "<<result<<"\n";
            std::exit(1);
        }
        std::cout<< "PASSED CASE "<<i<<"\n";
        ++i;
    }
    return 0;
}