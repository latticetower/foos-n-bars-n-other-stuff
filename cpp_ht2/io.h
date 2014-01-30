#pragma once
#include <iostream>
#include <memory>
#include "basic_types.h"
#include "lexer.h"
#include "base.h"
#include "context.h"

class ReadOp:public IOp {
  TokenInfo _variable;
public:
  ReadOp(TokenInfo var): _variable(var) { setLastError(OK); }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    setLastError(OK);
    int expr_result = 0;
    std::cin >> expr_result;
    if (context == NULL) {
      setLastError(UNKNOWN);
      return 0;
    }
    context->setVariable(_variable.token, expr_result);
    // TODO: should create new variable in current context
    return expr_result;
  }

  void print() {
    std::cout << "ReadOp:" << _variable.token << std::endl; 
  }
  bool valid() {
    return _variable.type == VAR;
  }
};

class PrintOp: public IOp {
  std::unique_ptr<IOp> _value;
public:
  PrintOp() {}
  PrintOp(IOp* op): _value(op) { setLastError(OK); }
  
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int expr_result = _value->Compute(context, _functions);
    if (_value->getLastError() != OK) {
      setLastError(_value->getLastError());
      return 0;
    }
    std::cout << expr_result << std::endl;
    setLastError(OK);
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

