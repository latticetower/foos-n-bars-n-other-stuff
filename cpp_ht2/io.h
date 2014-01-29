#pragma once
#include <iostream>
#include "basic_types.h"
#include "lexer.h"
#include "base.h"
#include "context.h"

class ReadOp:public IOp {
  TokenInfo _variable;
public:
  ReadOp(TokenInfo var): _variable(var) { }

  int Compute(IContext* context) {
    setLastError(ErrorType::OK);
    int expr_result = 0;
    std::cin >> expr_result;
    if (context == NULL) {
      setLastError(ErrorType::UNKNOWN);
      return 0;
    }
    context->setVariable(_variable.second, expr_result);
    // TODO: should create new variable in current context
    return expr_result;
  }

  void print() {
    std::cout << "ReadOp:" << _variable.second << std::endl; 
  }
  bool valid() {
    return _variable.first == TokenType::VAR;
  }
};

class PrintOp:public IOp {
  IOp * _value;
public:
  PrintOp(): _value(NULL) {}
  PrintOp(IOp* op): _value(op) {  }
  
  int Compute(IContext* context) {
    int expr_result = _value->Compute(context);
    if (_value->getLastError() != ErrorType::OK) {
      setLastError(_value->getLastError());
      return 0;
    }
    std::cout << expr_result << std::endl;
    setLastError(ErrorType::OK);
    return expr_result;
  }

  void print() {
    std::cout << "PrintOp:" << std::endl;
    _value->print();    
  }

  bool valid() {
    return  _value->valid();
  }
};

