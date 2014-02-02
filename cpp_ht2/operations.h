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
  std::unique_ptr<IOp> const & op1() const {
    return _op1;
  }
 
  std::unique_ptr<IOp> const & op2() const {
    return _op2;
  }

  ~PlusOp() { }
  PlusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  
    setLastError(OK);
    if (op1->getLastError() != OK) {
      setLastError(op1->getLastError()); 
      setErrorInfo(op1->getErrorInfo());
      return;
    }
    if (op2->getLastError() != OK) {
      setLastError(op2->getLastError()); 
      setErrorInfo(op2->getErrorInfo());
      return;
    }
  }

  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  void kickUpVars(std::set<std::string>* target) {
    _op1->kickUpVars(target);
    _op2->kickUpVars(target);
  }
};

class MinusOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  std::unique_ptr<IOp> const & op1() const {
    return _op1;
  }
 
  std::unique_ptr<IOp> const & op2() const {
    return _op2;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }
  ~MinusOp() { }
  MinusOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  
    setLastError(OK);
    if (op1->getLastError() != OK) {
      setLastError(op1->getLastError()); 
      setErrorInfo(op1->getErrorInfo());
      return;
    }
    if (op2->getLastError() != OK) {
      setLastError(op2->getLastError()); 
      setErrorInfo(op2->getErrorInfo());
      return;
    }
  }

  void kickUpVars(std::set<std::string>* target) {
    _op1->kickUpVars(target);
    _op2->kickUpVars(target);
  }
};

class MultOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  std::unique_ptr<IOp> const & op1() const {
    return _op1;
  }
 
  std::unique_ptr<IOp> const & op2() const {
    return _op2;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
   return visitor->visit(this);
  }

  ~MultOp() { }
  MultOp(IOp* op1, IOp* op2):_op1(op1), _op2(op2) {  
    setLastError(OK);
    if (op1->getLastError() != OK) {
      setLastError(op1->getLastError()); 
      setErrorInfo(op1->getErrorInfo());
      return;
    }
    if (op2->getLastError() != OK) {
      setLastError(op2->getLastError()); 
      setErrorInfo(op2->getErrorInfo());
      return;
    }
  }
  
  void kickUpVars(std::set<std::string>* target) {
    _op1->kickUpVars(target);
    _op2->kickUpVars(target);
  }
};

class DivideOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
public:
  std::unique_ptr<IOp> const & op1() const {
    return _op1;
  }
 
  std::unique_ptr<IOp> const & op2() const {
    return _op2;
  }
  ~DivideOp() { }
  DivideOp(IOp* op1, IOp* op2): _op1(op1), _op2(op2) {  
    setLastError(OK);
    if (op1->getLastError() != OK) {
      setLastError(op1->getLastError()); 
      setErrorInfo(op1->getErrorInfo());
      return;
    }
    if (op2->getLastError() != OK) {
      setLastError(op2->getLastError()); 
      setErrorInfo(op2->getErrorInfo());
      return;
    }
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  void kickUpVars(std::set<std::string>* target) {
    _op1->kickUpVars(target);
    _op2->kickUpVars(target);
  }
};


class AssignOp: public IOp {
  TokenInfo _variable;
  std::unique_ptr<IOp> _value;
public:
  TokenInfo const & variable() const {
    return _variable;
  }
  std::unique_ptr<IOp> const & value() const {
    return _value;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  AssignOp(TokenInfo var, IOp* value = NULL): _variable(var), _value(value) {  
    setLastError(OK);
    if (var.type != VAR) {
      setLastError(SYNTAX); 
      setErrorInfo(ErrorInfo(var.line));
      return;
    }
    if (value->getLastError() != OK) {
      setLastError(value->getLastError()); 
      setErrorInfo(value->getErrorInfo());
      return;
    }
  }

  void kickUpVars(std::set<std::string>* target) {
    if (_variable.type == VAR)
      target->insert(_variable.token);
    _value->kickUpVars(target);
  }
  ~AssignOp() { }
};

