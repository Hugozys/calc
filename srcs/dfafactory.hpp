#ifndef SRCS_DFAFACTORY_HPP_
#define SRCS_DFAFACTORY_HPP_
#include "dfa.hpp"
namespace dfa {
class DfaFactory {
 public:
  virtual Dfa CreateDfa() = 0;
  virtual ~DfaFactory() = default;
};
}  // namespace dfa
#endif  //  SRCS_DFAFACTORY_HPP_
