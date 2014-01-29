#pragma once
#include <iostream>
#include "lexer.h"
#include "base.h"

class ReadOp:public IOp {
  TokenInfo _variable;
public:
  ReadOp(TokenInfo var): _variable(var) { }

  int Compute() {
    int expr_result;
    std::cin >> expr_result;
    // TODO: should create new variable in current context
    return expr_result;
  }

  virtual void print() {
    std::cout << "ReadOp:" << _variable.second << std::endl; 
  }
  bool valid() {
    return _variable.first == TokenType::VAR;
  }
};

class PrintOp:public IOp {
  IOp * _value;
public:
  PrintOp(IOp* op): _value(op) {  }
  
  int Compute() {
    int expr_result = _value->Compute();
    std::cout << expr_result << std::endl;
    return expr_result;
  }

  virtual void print() {
    std::cout << "PrintOp:" << std::endl;
    _value->print();    
  }

  bool valid() {
    return  _value->valid();
  }
};

