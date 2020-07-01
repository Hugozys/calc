#include "nodenamegen.hpp"
#include "parser.hpp"

void Parser::PrintTreeVisitor::printExp(const char * label, const Exp & exp){
     auto name = NodeNameGen::GetNewNodeName();
     _nodeMap[&exp] = name;
     _out<< '\t'<<name<< "[label = \""<< label <<"\"]\n";
     if (exp.GetParent() != nullptr){
         _out << '\t'<< _nodeMap[exp.GetParent()] << " -> " << name<< '\n';
     }
}

 void Parser::PrintTreeVisitor::VisitPlusExp(const Parser::PlusExp & exp){
     printOpExp<PlusExp>("+", exp);
 }
 void Parser::PrintTreeVisitor::VisitMinusExp(const Parser::MinusExp & exp){
     printOpExp<MinusExp>("-", exp);
 }
 void Parser::PrintTreeVisitor::VisitMulExp(const Parser::MulExp & exp){
     printOpExp<MulExp>("x", exp);
 }
 void Parser::PrintTreeVisitor::VisitDivExp(const Parser::DivExp & exp){
     printOpExp<DivExp>("/", exp);
 }

 void Parser::PrintTreeVisitor::VisitExpoExp(const Parser::ExpoExp & exp){
     printOpExp<ExpoExp>("^", exp);
 }
 void Parser::PrintTreeVisitor::VisitIntExp(const Parser::IntExp & exp){
     printExp(exp.Value().c_str(), exp);
 }
 void Parser::PrintTreeVisitor::VisitFloatExp(const Parser::FloatExp & exp){
    printExp(exp.Value().c_str(), exp);
 }