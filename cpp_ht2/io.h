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

  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  if (context == NULL) {
  //    return ResultInfo(0, _variable.line, NO_CONTEXT);
  //  }
  //  if (!context->hasVariable(_variable.token)) {
  //    return ResultInfo(0, _variable.line, UNDEF_VARIABLE, _variable.token);
  //  }
  //  int expr_result = 0;
  //  std::cin >> expr_result;

  //  context->setVariable(_variable.token, expr_result);
  //  return ResultInfo(expr_result, _variable.line);
  //}

  void print(std::ostream& os) {
    os << "ReadOp:" << _variable.token << std::endl; 
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
  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  ResultInfo expr_result = _value->Compute(context, _functions);
  //  if (expr_result.error_type() != OK) {
  //    return expr_result;
  //  }
  //  std::cout << expr_result.result << std::endl;
  //  return expr_result;
  //}

  void print(std::ostream& os) {
    os << "PrintOp:" << std::endl;
    _value->print(os);    
  }

  void kickUpVars(std::set<std::string>* target) {
    _value->kickUpVars(target);
  }
};

