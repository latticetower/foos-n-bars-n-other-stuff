#pragma once
#include <iostream>
#include <vector>
#include "basic_types.h"
#include "lexer.h"
#include "base.h"

class ConditionOp: public IOp {
  IOp * _op1, * _op2;
  TokenInfo _oper;
public:
  ConditionOp(IOp* op1, IOp* op2, TokenInfo oper): _op1(op1), _op2(op2), _oper(oper) {  }
  
  int Compute(IContext* context) {
    int op1_value = _op1->Compute(context);
    int op2_value = _op2->Compute(context);
    setLastError(ErrorType::OK);
    switch (_oper.first) {
    case TokenType::EQ:
      return op1_value == op2_value;
      break;
    case TokenType::NEQ:
      return op1_value != op2_value;
      break;
    case TokenType::GREATER:
      return op1_value > op2_value;
      break;
    case TokenType::GEQ:
      return op1_value >= op2_value;
      break;
    case TokenType::LESS:
      return op1_value < op2_value;
      break;
    case TokenType::LEQ:
      return op1_value <= op2_value;
      break;
    default:
      setLastError(ErrorType::UNKNOWN);
      return -1; //TODO: should return some error instead
    }
    setLastError(ErrorType::UNKNOWN);
    return -1;
  }

  virtual void print() {
    std::cout << "ConditionOp:" << std::endl;
    _op1->print();
    std::cout << " condition op2:"<< std::endl;
    _op2->print();
  }

  bool valid() {
    return  _op1->valid() && _op2->valid();
  }
};

class IfOp: public IOp {
  IOp* _condition;
  std::vector<IOp*> _statements;
public:
  IfOp(IOp* cond, std::vector<IOp*> statements): _condition(cond), _statements(statements) {  }
  
  int Compute(IContext* context) {
    //TODO: add error handling
    int condition_result = _condition->Compute(context);
    if (condition_result) {
      for (std::vector<IOp*>::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
        (*iter)->Compute(context);
        if ((*iter)->getLastError() != ErrorType::OK) {
          setLastError((*iter)->getLastError());
          return 0;
        }
      }
    }
    setLastError(ErrorType::OK);
    return 0;
  }

  virtual void print() {
    std::cout << "If:" << std::endl;
    _condition->print();
    std::cout << "if lines: "<< std::endl;
    for (std::vector<IOp*>::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
        (*iter)->print();
      }
    
  }

  bool valid() {
    return  _condition->valid();
    //TODO: should validate contents too
  }
};

class WhileOp: public IOp {
  IOp* _condition;
  std::vector<IOp*> _statements;
public:
  WhileOp(IOp* cond, std::vector<IOp*> statements): _condition(cond), _statements(statements) {  }
  
  int Compute(IContext* context) {
    while (_condition->Compute(context)) {
      for (std::vector<IOp*>::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->Compute(context);
        if ((*iter)->getLastError() != ErrorType::OK) {
          setLastError((*iter)->getLastError());
          return 0;
        }
      }
    }
    setLastError(ErrorType::OK);
    return 0;
  }

  virtual void print() {
    std::cout << "while:" << std::endl;
    _condition->print();
    std::cout << "while statements: "<<std::endl;
    for (std::vector<IOp*>::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
        (*iter)->print();
    }
    
  }

  bool valid() {
    return  _condition->valid();
    //TODO: should validate contents too
  }
};