#ifndef SRCS_NODENAMEGEN_HPP_
#define SRCS_NODENAMEGEN_HPP_
#include <string>
class NodeNameGen {
 public:
  static std::string& GetNewNodeName() {
    for (std::size_t i = 0; i < _curNodeName.size(); ++i) {
      if (_curNodeName[i] < 'z') {
        ++_curNodeName[i];
        break;
      }
      _curNodeName[i] = 'a';
      if (i == _curNodeName.size() - 1) {
        _curNodeName.push_back('a');
        break;
      }
    }
    return _curNodeName;
  }

 private:
  static std::string _curNodeName;
};

std::string NodeNameGen::_curNodeName{'`'};  //  NOLINT(runtime/string)

#endif  //  SRCS_NODENAMEGEN_HPP_
