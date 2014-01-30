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
  void setLastError(ErrorType last_error) { _last_error = last_error; }
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

  ErrorType getLastError(){  return _last_error;  }

  virtual ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) = 0;
  virtual void print(std::ostream& os) = 0;
  
  virtual const ErrorInfo getErrorInfo() {
    return _info;
  }

  virtual void setErrorInfo(ErrorInfo ei) {
    _info.line = ei.line;
  }

};



class BasicOp: public IOp {
  TokenInfo _value;
public:
  BasicOp(TokenInfo ti): _value(ti) { 
    setLastError(OK); 
  }

  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    if (_value.type == NUMBER) {
      return ResultInfo(atoi(_value.token.c_str()), _value.line);
    } 
    else {
      if (_value.type == VAR) {
        if (context != NULL) {
          //TODO: should also check here function names. or not)
          int vi = context->getValue(_value.token);
          //TODO: do something with context errors
          return ResultInfo(vi, _value.line, context->getLastError());
        }
      }
    }
    return ResultInfo(0, _value.line, UNDEF_VARIABLE);
  }

  void print(std::ostream& os) {
    os << "BasicOp: " << _value.token << std::endl;
  }
};