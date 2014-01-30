#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "basic_types.h"
#include "lexer.h"
#include "base.h"

class ConditionOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
  TokenInfo _oper;
public:
  ConditionOp(IOp* op1, IOp* op2, TokenInfo oper): _op1(op1), _op2(op2), _oper(oper) { }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int op1_value = _op1->Compute(context, _functions);
    int op2_value = _op2->Compute(context, _functions);
    setLastError(OK);
    switch (_oper.type) {
    case EQ:
      return op1_value == op2_value;
      break;
    case NEQ:
      return op1_value != op2_value;
      break;
    case GREATER:
      return op1_value > op2_value;
      break;
    case GEQ:
      return op1_value >= op2_value;
      break;
    case LESS:
      return op1_value < op2_value;
      break;
    case LEQ:
      return op1_value <= op2_value;
      break;
    default:
      setLastError(UNKNOWN);
      return -1; //TODO: should return some error instead
    }
    setLastError(UNKNOWN);
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
  std::unique_ptr<IOp> _condition;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  IfOp(IOp* cond, std::vector<IOp* > statements): _condition(cond) { 
    setLastError(OK);
    _statements.resize(statements.size());
    for (int i = 0; i < statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
  }
  
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    //TODO: add error handling
    int condition_result = _condition->Compute(context, _functions);
    if (condition_result) {
      for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
        (*iter)->Compute(context, _functions);
        if ((*iter)->getLastError() != OK) {
          setLastError((*iter)->getLastError());
          return 0;
        }
      }
    }
    setLastError(OK);
    return 0;
  }

  virtual void print() {
    std::cout << "If:" << std::endl;
    _condition->print();
    std::cout << "if lines: "<< std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
        (*iter)->print();
      }
    
  }

  bool valid() {
    return  _condition->valid();
    //TODO: should validate contents too
  }
};

class WhileOp: public IOp {
  std::unique_ptr<IOp> _condition;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  WhileOp(IOp* cond, std::vector<IOp* > statements): _condition(cond) { 
    setLastError(OK);
    _statements.resize(statements.size());
    for (int i = 0; i < statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
  }
  
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    while (_condition->Compute(context, _functions)) {
      for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->Compute(context, _functions);
        if ((*iter)->getLastError() != OK) {
          setLastError((*iter)->getLastError());
          return 0;
        }
      }
    }
    setLastError(OK);
    return 0;
  }

  virtual void print() {
    std::cout << "while:" << std::endl;
    _condition->print();
    std::cout << "while statements: "<<std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->print();
    }
    
  }

  bool valid() {
    return  _condition->valid();
    //TODO: should validate contents too
  }
};