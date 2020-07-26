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

void assertString(const std::string & result, const std::pair<std::string, std::string> & expected){
    if(! result.compare(expected.first)){
            std::cout << "input: " << expected.first;
            std::cout << "expected: "<<expected.second<<"\n";
            std::cout << "but  get: "<<result<<"\n";
            std::exit(1);
    }
}
int main(void){
    std::unordered_map<std::string, std::string> correct_test_suite{
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
        {"()", "LPARA RPARA"},
        {"1+2", "INT( 1 ) PLUS INT( 2 )"},
        {"1x2", "INT( 1 ) MUL INT( 2 )"},
        {"1-2", "INT( 1 ) MINUS INT( 2 )"},
        {"-1x3", "MINUS INT( 1 ) MUL INT( 3 )"},
        {"1/2", "INT( 1 ) DIV INT( 2 )"},
        {"1^2", "INT( 1 ) EXP INT( 2 )"},
        {"(1+2)()+3", "LPARA INT ( 1 ) PLUS INT ( 2 ) WS LPARA RPARA PLUS INT ( 3 )"},
        {"1+3/4x5-7^8/9x3.14159\n", "INT( 1 ) PLUS INT( 3 ) DIV INT( 4 ) MUL INT( 5 ) MINUS INT( 7 ) EXP INT( 8 ) DIV INT( 9 ) MUL FLOAT( 3.14159 ) END"},
        {"1+3 x   4 -   5 \n", "INT( 1 ) PLUS INT( 3 ) MUL WS INT( 4 ) WS MINUS WS INT( 5 ) WS END"},
        {"(( 3.2 + 4.97)x27 / -9 )^(1/5)\n", "LPARA LPARA WS FLOAT( 3.2 ) WS PLUS WS FLOAT( 4.97 ) RPARA MUL INT( 27 ) WS DIV WS MINUS INT( 9 ) WS RPARA EXP LPARA INT( 1 ) DIV INT( 5 ) RPARA END"},      
    };
    std::unordered_map<std::string, std::string> error_test_suite{
        {"1+3*2", "Invalid token at index 3: *"},
        {"12.34fgh", "Invalid token at index 5: f"},
        {"((((", "Unmatched parentheses at index 3"},
        {"(1+2)x3)", "Unmatched parentheses at index 7"},
        {")1+2", "Unmatched parentheses at index 0"}  
    };
    std::cout<<std::string(10, '=')<<"Test Correct Cases"<<std::string(10, '=')<<'\n';
    TokenGen sut{};
    int i = 1;
    for(const auto & pair : correct_test_suite){
        auto tokenList{sut.Tokenize(pair.first)};
        auto result{print(tokenList)};
        assertString(result, pair); 
        std::cout<< "PASSED CASE "<<i<<"\n";
        ++i;
    }

    std::cout<<std::string(10, '=')<<"Test Error Cases"<<std::string(10, '=')<<'\n';
    for (const auto & pair: error_test_suite){
        try{
            sut.Tokenize(pair.first);
        }
        catch (const std::exception & e){
            assertString(e.what(), pair);
        }
        std::cout<< "PASSED CASE"<<i<<'\n';
        ++i;
    }

    return 0;
}
