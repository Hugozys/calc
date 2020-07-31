#include <algorithm>
#include <cmath>
#include <sstream>

#include "exception.hpp"
#include "nodenamegen.hpp"
#include "parser.hpp"

void Parser::PrintTreeVisitor::printExp(const char *label, const Exp &exp) {
  auto name = NodeNameGen::GetNewNodeName();
  _nodeMap[&exp] = name;
  _out << '\t' << name << "[label = \"" << label << "\"]\n";
  if (exp.GetParent() != nullptr) {
    _out << '\t' << _nodeMap[exp.GetParent()] << " -> " << name << '\n';
  }
}

void Parser::PrintTreeVisitor::VisitPlusExp(const Parser::PlusExp &exp) {
  printOpExp<PlusExp>("+", exp);
}
void Parser::PrintTreeVisitor::VisitMinusExp(const Parser::MinusExp &exp) {
  printOpExp<MinusExp>("-", exp);
}
void Parser::PrintTreeVisitor::VisitMulExp(const Parser::MulExp &exp) {
  printOpExp<MulExp>("x", exp);
}
void Parser::PrintTreeVisitor::VisitDivExp(const Parser::DivExp &exp) {
  printOpExp<DivExp>("/", exp);
}

void Parser::PrintTreeVisitor::VisitExpoExp(const Parser::ExpoExp &exp) {
  printOpExp<ExpoExp>("^", exp);
}
void Parser::PrintTreeVisitor::VisitIntExp(const Parser::IntExp &exp) {
  printExp(exp.Value().c_str(), exp);
}
void Parser::PrintTreeVisitor::VisitFloatExp(const Parser::FloatExp &exp) {
  printExp(exp.Value().c_str(), exp);
}

bool Parser::EvalVisitor::IsZero(double num) const {
  double diff{std::abs(num - 0.0)};
  if (diff <= 1e-12) return true;

  // Otherwise fall back to Knuth's algorithm
  return (diff <= (std::max(std::abs(num), std::abs(0.0)) * 1e-6));
}

void Parser::EvalVisitor::VisitPlusExp(const PlusExp &exp) {
  auto it = exp.CBegin();
  double value{};
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    auto expPt = (*it).get();
    value += _evalMap[expPt];
    ++it;
  }
  _value = _evalMap[&exp] = value;
}

void Parser::EvalVisitor::VisitMinusExp(const MinusExp &exp) {
  auto it = exp.CBegin();
  (*(*it)).Accept(*this);
  auto expPt = (*it).get();
  ++it;
  double value{exp.GetChildrenNum() == 1 ? -_evalMap[expPt] : _evalMap[expPt]};
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    auto expPt = (*it).get();
    value -= _evalMap[expPt];
    ++it;
  }
  _value = _evalMap[&exp] = value;
}

void Parser::EvalVisitor::VisitMulExp(const MulExp &exp) {
  auto it = exp.CBegin();
  double value{1};
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    auto expPt = (*it).get();
    value *= _evalMap[expPt];
    ++it;
  }
  _value = _evalMap[&exp] = value;
}

void Parser::EvalVisitor::VisitDivExp(const DivExp &exp) {
  auto it = exp.CBegin();
  (*(*it)).Accept(*this);
  auto expPt = (*it).get();
  double value{_evalMap[expPt]};
  ++it;
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    auto expPt = (*it).get();
    if (IsZero(_evalMap[expPt])) {
      throw DivideByZero{exp.Pos()};
    }
    value /= _evalMap[expPt];
    ++it;
  }
  _value = _evalMap[&exp] = value;
}

void Parser::EvalVisitor::VisitExpoExp(const ExpoExp &exp) {
  auto it = exp.CBegin();
  (*(*it)).Accept(*this);
  auto expPt = (*it).get();
  double value{_evalMap[expPt]};
  ++it;
  while (it != exp.CEnd()) {
    (*(*it)).Accept(*this);
    auto expPt = (*it).get();
    value = pow(value, _evalMap[expPt]);
    ++it;
  }
  _value = _evalMap[&exp] = value;
}

void Parser::EvalVisitor::VisitIntExp(const IntExp &exp) {
  _value = _evalMap[&exp] = std::stol(exp.Value());
  return;
}

void Parser::EvalVisitor::VisitFloatExp(const FloatExp &exp) {
  _value = _evalMap[&exp] = std::stod(exp.Value());
}
