#pragma once
#include <utility>
#include <iostream>
#include "basic_types.h"
#include "lexer.h"
#include "context.h"
#include "ivisitor.h"

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
  std::string const& getName()const {
    return _name;
  }

  ErrorType getLastError(){  return _last_error;  }

  virtual void kickUpVars(std::set<std::string>* target) = 0;
  virtual ResultInfo acceptVisitor(IVisitor * visitor) = 0;

  virtual const ErrorInfo getErrorInfo() {
    return _info;
  }

  virtual void setErrorInfo(ErrorInfo ei) {
    _info.line = ei.line;
  }
  virtual ~IOp() { };

};



class BasicOp: public IOp {
  TokenInfo _value;
public:
  BasicOp(TokenInfo ti): _value(ti) { 
    setLastError(OK); 
  }

  TokenInfo const & get_value() const {
    return _value;
  }

  void kickUpVars(std::set<std::string>* target) {
    if (_value.type == VAR)
      target->insert(_value.token);
  }
  
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  ~BasicOp() {  }
};