#pragma once
#include <utility>
#include <iostream>
#include "basic_types.h"
#include "lexer.h"
#include "context.h"
// #include "error_handling.h"

class IOp{
  ErrorType _last_error;
protected:
  void setLastError(ErrorType last_error) {
    _last_error = last_error;
  }
public:
  IOp() {
    setLastError(ErrorType::UNKNOWN); 
  }
  

  ErrorType getLastError(){
    return _last_error;
  }

  virtual int Compute(IContext* context) = 0;
  virtual void print() = 0;
  
  bool valid() {
    return true;
  }
};



class BasicOp: public IOp {
  TokenInfo _value;
public:
  BasicOp(TokenInfo ti): _value(ti) { 
    setLastError(ErrorType::OK); 
  }

  int Compute(IContext* context) {
    if (_value.first == TokenType::NUMBER) {
      setLastError(ErrorType::OK);
      return atoi(_value.second.c_str());
    } 
    else {
      if (_value.first == TokenType::VAR) {
        if (context != NULL) {
          //TODO: should also check here function names. or not)
          int vi = context->getValue(_value.second);
          setLastError(context->getLastError());
          return vi;
        }
      }
    }
    setLastError(ErrorType::UNDEF_VARIABLE);
    return 0;
  }

  void print() {
    std::cout << "BasicOp: " << _value.second << std::endl;
  }
  bool valid() {
    return true;
  }
};