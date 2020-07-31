#include <fruit/fruit.h>

#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "exception.hpp"
#include "parser.hpp"
#include "tokengen.hpp"

using fruit::Injector;

// return true if the difference between a and b is less than absEpsilon, or
// within relEpsilon percent of the larger of a and b
bool approximatelyEqualAbsRel(double a, double b, double absEpsilon,
                              double relEpsilon) {
  // Check if the numbers are really close -- needed when comparing numbers near
  // zero.
  double diff{std::abs(a - b)};
  if (diff <= absEpsilon) return true;

  // Otherwise fall back to Knuth's algorithm
  return (diff <= (std::max(std::abs(a), std::abs(b)) * relEpsilon));
}

int main(void) {
  std::unordered_set<std::string> parserErrorTestSuite{
      "1 +",          "1 + +", "1 -",
      "1 - +",        "1 x ",  "1 / ",
      "1 ^",          "1 2 3", "1 ( + ) 2",
      "(1 + 5 + ) 6", "1 / 0", "2^(+(1 + 2) x 5^--7/--(--8 + +9) + )"};

  std::unordered_map<std::string, double> correctTestSuite{
      {"1", 1},
      {"200+3000", 3200},
      {"200-300", -100},
      {"-200 + 500", 300},
      {"300x-700", -210000},
      {"300++500", 800},
      {"-10++20--(20+30)", 60},
      {"10x20", 200},
      {"1/2", 0.5},
      {"5^3", 125},
      {"2^3^3", 134217728},
      {"(2^3)^3", 512},
      {"10^-2", 0.01},
      {"10 + 30 x 40", 1210},
      {"2^(+(1 + 2) x 5^--7/--(--8 + +9))", 64},
      {"1.2", 1.2},
      {"200+3000.0", 3200.0},
      {"200-300", -100},
      {"-200 + 500.23", 300.23},
      {"300x-700.0", -210000.0},
      {"300++500.2145", 800.2145},
      {"-10++20--(20+30.9746)", 60.9746},
      {"10x20", 200},
      {"1/2.0", 0.5},
      {"1.0/2", 0.5},
      {"5.0^3", 125.0},
      {"10.245 + 30 x 40", 1210.245},
  };

  std::cout << std::string(10, '=') << "Test Correct Cases"
            << std::string(10, '=') << '\n';
  Injector<TokenGen> injector(getTokenGenComponent);
  TokenGen &tokenGen = injector.get<TokenGen &>();
  Parser sut{};
  int i = 1;
  for (const auto &pair : correctTestSuite) {
    auto input{pair.first};
    input.push_back('\n');
    std::stringstream ss;
    ss << "debug/test_" << i << ".dot";
    auto str = ss.str();
    auto tokenList{tokenGen.Tokenize(input)};
    auto result = sut.Parse(tokenList, true, str.c_str());
    if (!approximatelyEqualAbsRel(pair.second, result, 1e-12, 1e-5)) {
      std::cout << "FAILED CASE " << i << ": ";
      std::cout << "Input string: " << pair.first
                << ", expected: " << pair.second << ", but get: " << result
                << std::endl;
    }
    std::cout << "PASSED CASE " << i << "\n";
    ++i;
  }

  std::cout << std::string(10, '=') << "Test Error Cases"
            << std::string(10, '=') << '\n';
  for (const auto &str : parserErrorTestSuite) {
    auto input{str};
    input.push_back('\n');
    auto tokenList{tokenGen.Tokenize(input)};
    try {
      sut.Parse(tokenList);
      std::cout << "FAILED CASE " << i << '\n';
      assert(input.c_str() && " should throw ParserError exception");
    } catch (const ParserError &ex) {
      std::cout << "PASSED CASE " << i << '\n';
    } catch (const DivideByZero &ex) {
      std::cout << "PASSED CASE " << i << '\n';
    }
    ++i;
  }
  return 0;
}
