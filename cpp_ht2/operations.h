#pragma once
#include "lexer.h"
#include <iostream>
#include <vector>
#include "base.h"

class PlusOp: public IOp {
  IOp *_op1, *_op2;
public:
  PlusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  }

  int Compute() {
    int op1_value = _op1->Compute();
    int op2_value = _op2->Compute();
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
  IOp *_op1, *_op2;
public:
  MinusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  }

  int Compute() {
    int op1_value = _op1->Compute();
    int op2_value = _op2->Compute();
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
  IOp *_op1, *_op2;
public:
  MultOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  }

  int Compute() {
    int op1_value = _op1->Compute();
    int op2_value = _op2->Compute();
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
  IOp *_op1, *_op2;
public:
  DivideOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  }
  int Compute() {
    int op1_value = _op1->Compute();
    int op2_value = _op2->Compute();
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
  IOp *_value;
public:
  AssignOp(TokenInfo var, IOp* value): _variable(var), _value(value) {  }
  int Compute() {
    int var_value = _value->Compute();
    //TODO: add variable context
    return var_value;
  }
  virtual void print() {
    std::cout << "Assign: " << std::endl;
    std::cout << " var name: " << _variable.second << std::endl;
    std::cout << " Assign op value: " << std::endl;
    _value->print();
  }
  bool valid() {
    return  _variable.first == TokenType::VAR && _value->valid();
  }
};

