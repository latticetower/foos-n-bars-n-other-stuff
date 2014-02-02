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
  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  ResultInfo op1_value = _op1->Compute(context, _functions);
  //  ResultInfo op2_value = _op2->Compute(context, _functions);
  //  return op1_value + op2_value;
  //}

  void print(std::ostream& os) {
    os << "PlusOp: " << std::endl;
    os << " PlusOp op1: " << std::endl;
    _op1->print(os);
    os << " PlusOp op2: " << std::endl;
    _op2->print(os);
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

  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  ResultInfo op1_value = _op1->Compute(context, _functions);
  //  ResultInfo op2_value = _op2->Compute(context, _functions);
  //  return op1_value - op2_value;
  //}

  void print(std::ostream& os) {
    os << "MinusOp: " << std::endl;
    os << " MinusOp op1: " << std::endl;
    _op1->print(os);
    os << " MinusOp op2: " << std::endl;
    _op2->print(os);
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

  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  ResultInfo op1_value = _op1->Compute(context, _functions);
  //  ResultInfo op2_value = _op2->Compute(context, _functions);
  //  return op1_value * op2_value;
  //}

  void print(std::ostream& os) {
    os << "MultOp: " << std::endl;
    os << " MultOp op1: " << std::endl ;
    _op1->print(os);
    os << " MultOp op2: "<< std::endl ;
    _op2->print(os);
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
  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  ResultInfo op1_value = _op1->Compute(context, _functions);
  //  ResultInfo op2_value = _op2->Compute(context, _functions);
  //  return op1_value / op2_value;
  //}

  virtual void print(std::ostream& os) {
    os << "DivideOp: " << std::endl;
    os << " DivideOp op1: " << std::endl;
    _op1->print(os);
    os << " DivideOp op2: " << std::endl;
    _op2->print(os);
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
 
  //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  //  if (context == NULL) {
  //    return ResultInfo(0, _variable.line, NO_CONTEXT);
  //  }
  //  ResultInfo var_value = _value->Compute(context, _functions);
  //  if (var_value.error_type() != OK) {
  //    return var_value;
  //  }
  //  
  //  context->setVariable(_variable.token, var_value.result);
  //  return var_value;
  //}

  void print(std::ostream& os) {
    os << "Assign: " << std::endl;
    os << " var name: " << _variable.token << std::endl;
    os << " Assign op value: " << std::endl;
    _value->print(os);
  }

  void kickUpVars(std::set<std::string>* target) {
    if (_variable.type == VAR)
      target->insert(_variable.token);
    _value->kickUpVars(target);
  }
  ~AssignOp() { }
};

