#ifndef SRCS_PARSER_HPP_
#define SRCS_PARSER_HPP_
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "token.hpp"

enum class ExpType { OP, PRIMITIVE };

class Parser {
 public:
  class SyntaxTree;
  double Parse(const std::queue<PToken> &tokenPool, bool debugMode = false,
               const char *filename = nullptr);

 private:
  class Exp;
  class OpExp;
  class PlusExp;
  class MinusExp;
  class MulExp;
  class DivExp;
  class ExpoExp;
  class IntExp;
  class FloatExp;
  class Visitor;
  class PrintTreeVisitor;
  class TypeCheckVisitor;
  class EvalVisitor;
  class ParserInternal;
};

class Parser::ParserInternal {
 public:
  TokenType CurrentType();
  PToken Current();
  bool Eat(TokenType type);
  std::shared_ptr<Exp> ParseS();
  std::shared_ptr<Exp> ParseE();
  std::shared_ptr<Exp> ParseT();
  std::shared_ptr<Exp> ParseF();
  std::shared_ptr<Exp> ParseN();
  std::shared_ptr<Exp> ParseK();
  explicit ParserInternal(const std::queue<PToken> &queue)
      : _tokenQueue{std::move(queue)} {}

 private:
  std::queue<PToken> _tokenQueue;
};

class Parser::Visitor {
 public:
  Visitor() = default;
  virtual ~Visitor() = default;
  virtual void VisitPlusExp(const PlusExp &exp) = 0;
  virtual void VisitMinusExp(const MinusExp &exp) = 0;
  virtual void VisitMulExp(const MulExp &exp) = 0;
  virtual void VisitDivExp(const DivExp &exp) = 0;
  virtual void VisitExpoExp(const ExpoExp &exp) = 0;
  virtual void VisitIntExp(const IntExp &exp) = 0;
  virtual void VisitFloatExp(const FloatExp &exp) = 0;
};

class Parser::PrintTreeVisitor : public Visitor {
 public:
  explicit PrintTreeVisitor(const char *filename)
      : Visitor{}, _out{filename}, _nodeMap{} {
    _out << "digraph G{\n";
  }
  virtual ~PrintTreeVisitor() {
    _out << "}\n";
    _out.close();
  }

  virtual void VisitPlusExp(const PlusExp &exp);
  virtual void VisitMinusExp(const MinusExp &exp);
  virtual void VisitMulExp(const MulExp &exp);
  virtual void VisitDivExp(const DivExp &exp);
  virtual void VisitExpoExp(const ExpoExp &exp);
  virtual void VisitIntExp(const IntExp &exp);
  virtual void VisitFloatExp(const FloatExp &exp);

 private:
  void printExp(const char *label, const Exp &exp);
  template <typename T>
  void printOpExp(const char *label, const T &exp);
  std::ofstream _out;
  std::unordered_map<const Exp *, std::string> _nodeMap;
};

template <typename T>
void Parser::PrintTreeVisitor::printOpExp(const char *label, const T &exp) {
  printExp(label, exp);
  auto it = exp.CBegin();
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    ++it;
  }
}

class Parser::EvalVisitor : public Visitor {
 public:
  EvalVisitor() : _value{}, _evalMap{} {}
  double Value() const { return _value; }
  virtual void VisitPlusExp(const PlusExp &exp);
  virtual void VisitMinusExp(const MinusExp &exp);
  virtual void VisitMulExp(const MulExp &exp);
  virtual void VisitDivExp(const DivExp &exp);
  virtual void VisitExpoExp(const ExpoExp &exp);
  virtual void VisitIntExp(const IntExp &exp);
  virtual void VisitFloatExp(const FloatExp &exp);

 private:
  double _value;
  std::unordered_map<const Exp *, double> _evalMap;
  bool IsZero(double num) const;
};

class Parser::SyntaxTree {
 public:
  const Exp &Root() const { return *_rootExp; }
  std::shared_ptr<Exp> _rootExp;
  explicit SyntaxTree(std::shared_ptr<Exp> root) : _rootExp{root} {}
};

class Parser::Exp {
 public:
  std::size_t Pos() const { return _pos; }
  ExpType Type() const { return _type; }
  void SetParent(const Exp *parent) { _parent = parent; }
  virtual std::size_t GetChildrenNum() const { return 0; }
  const Exp *GetParent() const { return _parent; }

  virtual ~Exp() = default;
  virtual void Accept(Visitor &visitor) const = 0;

 protected:
  Exp(ExpType type, std::size_t pos)
      : _parent{nullptr}, _type{type}, _pos{pos} {}

 private:
  const Exp *_parent;
  ExpType _type;
  std::size_t _pos;
};

class Parser::OpExp : public Exp {
 public:
  class ConstIterator;
  ConstIterator CBegin() const;
  ConstIterator CEnd() const;
  virtual ~OpExp() = default;
  std::size_t GetChildrenNum() const override { return _children.size(); }

 protected:
  OpExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : Exp{ExpType::OP, pos}, _children{list} {}

 private:
  using ExpIt = std::vector<std::shared_ptr<Exp>>::iterator;
  using ConstExpIt = std::vector<std::shared_ptr<Exp>>::const_iterator;
  std::vector<std::shared_ptr<Exp>> _children;
};

class Parser::OpExp::ConstIterator {
 public:
  friend Parser::OpExp::ConstIterator Parser::OpExp::CBegin() const;
  friend Parser::OpExp::ConstIterator Parser::OpExp::CEnd() const;
  ConstIterator &operator++() {
    ++_expIt;
    return *this;
  }

  std::shared_ptr<Exp> operator*() const { return *_expIt; }

  bool operator==(const ConstIterator &rhs) { return _expIt == rhs._expIt; }

  bool operator!=(const ConstIterator &rhs) { return _expIt != rhs._expIt; }

 private:
  explicit ConstIterator(ConstExpIt it) : _expIt{it} {};
  ConstExpIt _expIt;
};

class Parser::PlusExp : public OpExp {
 public:
  PlusExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : OpExp{pos, list} {}
  virtual ~PlusExp() = default;
  void Accept(Visitor &visitor) const override;
};

class Parser::MinusExp : public OpExp {
 public:
  MinusExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : OpExp{pos, list} {}
  virtual ~MinusExp() = default;
  void Accept(Visitor &visitor) const override;
};

class Parser::MulExp : public OpExp {
 public:
  MulExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : OpExp{pos, list} {}
  virtual ~MulExp() = default;
  virtual void Accept(Visitor &visitor) const;
};

class Parser::DivExp : public OpExp {
 public:
  DivExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : OpExp{pos, list} {}
  virtual ~DivExp() = default;
  void Accept(Visitor &visitor) const override;
};

class Parser::ExpoExp : public OpExp {
 public:
  ExpoExp(std::size_t pos, std::initializer_list<std::shared_ptr<Exp>> list)
      : OpExp{pos, list} {}
  virtual ~ExpoExp() = default;
  void Accept(Visitor &visitor) const override;
};

class Parser::IntExp : public Exp {
 public:
  const std::string &Value() const { return _value; }
  IntExp(std::size_t pos, const std::string &intstr)
      : Exp{ExpType::PRIMITIVE, pos}, _value{intstr} {}
  virtual ~IntExp() = default;
  void Accept(Visitor &visitor) const override;

 private:
  std::string _value;
};

class Parser::FloatExp : public Exp {
 public:
  const std::string &Value() const { return _value; }
  FloatExp(std::size_t pos, const std::string &floatstr)
      : Exp{ExpType::PRIMITIVE, pos}, _value{floatstr} {}
  virtual ~FloatExp() = default;
  void Accept(Visitor &visitor) const override;

 private:
  std::string _value;
};

#endif  //  SRCS_PARSER_HPP_
