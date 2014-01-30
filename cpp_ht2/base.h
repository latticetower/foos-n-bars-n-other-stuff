#pragma once
#include <utility>
#include <iostream>
#include "basic_types.h"
#include "lexer.h"
#include "context.h"
// #include "error_handling.h"

class IOp{
  ErrorType _last_error;
  ErrorInfo _info;
  std::string _name;
protected:
  void setLastError(ErrorType last_error) {
    _last_error = last_error;
  }
  void setName(std::string const & c) {
    _name = std::string(c);
  }
public:
  IOp() {
    setLastError(OK); 
    _name = "";
  }
  std::string const& getName() {
    return _name;
  }

  ErrorType getLastError(){
    return _last_error;
  }

  virtual int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) = 0;
  virtual void print() = 0;
  
  virtual const ErrorInfo getErrorInfo() {
    return _info;
  }

  virtual void setErrorInfo(ErrorInfo ei) {
  }

  bool valid() {
    return true;
  }
};



class BasicOp: public IOp {
  TokenInfo _value;
public:
  BasicOp(TokenInfo ti): _value(ti) { }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    if (_value.type == NUMBER) {
      setLastError(OK);
      return atoi(_value.token.c_str());
    } 
    else {
      if (_value.type == VAR) {
        if (context != NULL) {
          //TODO: should also check here function names. or not)
          int vi = context->getValue(_value.token);
          setLastError(context->getLastError());
          return vi;
        }
      }
    }
    setLastError(UNDEF_VARIABLE);
    return 0;
  }

  void print() {
    std::cout << "BasicOp: " << _value.token << std::endl;
  }
  bool valid() {
    return true;
  }
};