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
  TokenInfo const& variable() const {
    return _variable;
  }

  ~ReadOp() { }
  ReadOp(TokenInfo var): _variable(var) { 
    setLastError(OK);
    if (var.type != VAR) {
      setLastError(SYNTAX);
      setErrorInfo(var.line);
    }
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  void kickUpVars(std::set<std::string>* target) {
    if (_variable.type == VAR)
      target->insert(_variable.token);
  }
};

class PrintOp: public IOp {
  std::unique_ptr<IOp> _value;
public:
  std::unique_ptr<IOp> const & value() const {
    return _value;
  }
  ~PrintOp() { }
  PrintOp() {}
  PrintOp(IOp* op): _value(op) { 
    setLastError(op->getLastError()); 
    setErrorInfo(op->getErrorInfo());
  }
  
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  void kickUpVars(std::set<std::string>* target) {
    _value->kickUpVars(target);
  }
};

