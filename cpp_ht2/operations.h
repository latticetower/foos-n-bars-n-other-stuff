#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "basic_types.h"
#include "context.h"
#include "lexer.h"

#include "base.h"

class PlusOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  PlusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  setLastError(OK); }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int op1_value = _op1->Compute(context, _functions);
    int op2_value = _op2->Compute(context, _functions);
    setLastError(OK);
    return op1_value + op2_value;
  }

  void print() {
    std::cout << "PlusOp: " << std::endl;
    std::cout << " PlusOp op1: " << std::endl;
    _op1->print();
    std::cout << " PlusOp op2: " << std::endl;
    _op2->print();
  }
  bool valid() {
    return _op1->valid() && _op2->valid();
  }
};

class MinusOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  MinusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  setLastError(OK); }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int op1_value = _op1->Compute(context, _functions);
    if (_op1->getLastError() != OK) {
      setLastError(_op1->getLastError());
      return 0;
    }
    int op2_value = _op2->Compute(context, _functions);
    if (_op2->getLastError() != OK) {
      setLastError(_op2->getLastError());
      return 0;
    }
    setLastError(OK);
    return op1_value - op2_value;
  }

  void print() {
    std::cout << "MinusOp: " << std::endl;
    std::cout << " MinusOp op1: " << std::endl;
    _op1->print();
    std::cout << " MinusOp op2: " << std::endl;
    _op2->print();
  }

  bool valid() {
    return _op1->valid() && _op2->valid();
  }
};

class MultOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  MultOp(IOp* op1, IOp* op2):_op1(op1), _op2(op2) {  setLastError(OK); }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int op1_value = _op1->Compute(context, _functions);
    if (_op1->getLastError() != OK) {
      setLastError(_op1->getLastError());
      return 0;
    }
    int op2_value = _op2->Compute(context, _functions);
    if (_op2->getLastError() != OK) {
      setLastError(_op2->getLastError());
      return 0;
    }
    setLastError(OK);
    return op1_value * op2_value;
  }

  void print() {
    std::cout << "MultOp: " << std::endl;
    std::cout << " MultOp op1: " << std::endl ;
    _op1->print();
    std::cout << " MultOp op2: "<< std::endl ;
    _op2->print();
  }

  bool valid() {
    return _op1->valid() && _op2->valid();
  }
};

class DivideOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  DivideOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  setLastError(OK); }
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int op1_value = _op1->Compute(context, _functions);
    if (_op1->getLastError() != OK) {
      setLastError(_op1->getLastError());
      return 0;
    }
    int op2_value = _op2->Compute(context, _functions);
    if (_op2->getLastError() != OK) {
      setLastError(_op2->getLastError());
      return 0;
    }
    if (op2_value == 0) {
      setLastError(DIVISION_BY_ZERO);
      return 0;
    }
    setLastError(OK);
    return op1_value / op2_value;
  }

  virtual void print() {
    std::cout << "DivideOp: " << std::endl;
    std::cout << " DivideOp op1: " << std::endl;
    _op1->print();
    std::cout << " DivideOp op2: " << std::endl;
    _op2->print();
  }

  bool valid() {
    return _op1->valid() && _op2->valid();
  }
};


class AssignOp: public IOp {
  TokenInfo _variable;
  std::unique_ptr<IOp> _value;
public:
  AssignOp(TokenInfo var, IOp* value = NULL): _variable(var), _value(value) {  setLastError(OK); }
 

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    int var_value = _value->Compute(context, _functions);
    if (_value->getLastError() != OK) {
      setLastError(_value->getLastError());
      return 0;
    }
    if (context == NULL) {
      setLastError(UNKNOWN);
      return 0;
    }
    context->setVariable(_variable.token, var_value);
    setLastError(OK);
    //TODO: add variable context
    return var_value;
  }
  virtual void print() {
    std::cout << "Assign: " << std::endl;
    std::cout << " var name: " << _variable.token << std::endl;
    std::cout << " Assign op value: " << std::endl;
    _value->print();
  }
  bool valid() {
    return  _variable.type == VAR && _value->valid();
  }
};

