#pragma once
#include "lexer.h"
#include <iostream>

class IOp{
public:
  IOp() { }
  int Compute() {
    return 0;
  }
  void getLastError() {
  }
  virtual void print() =0;
  
  bool valid() {
    return true;
  }
};
class InvalidOp: public IOp {
public: 
  bool valid() {
    return false;
  }
  void print() {
    std::cout << "InvalidOp!" << std::endl;
  }
};

class BasicOp: public IOp {
  TokenInfo _value;
public:
  BasicOp(TokenInfo ti): _value(ti) {  }

  int Compute() {
    if (_value.first == TokenType::NUMBER) {
      return atoi(_value.second.c_str());
    }
    return 0;
  }

  void print() {
    std::cout << "BasicOp: " << _value.second << std::endl;
  }
  bool valid() {
    return true;
  }
};