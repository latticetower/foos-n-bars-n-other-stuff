#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "basic_types.h"
#include "lexer.h"
#include "base.h"
#include "ivisitor.h"

class ConditionOp: public IOp {
  std::unique_ptr<IOp> _op1, _op2;
  TokenInfo _oper;
public:
  ~ConditionOp() { }

  std::unique_ptr<IOp> const & op1() const {
    return _op1;
  }
  std::unique_ptr<IOp> const & op2() const {
    return _op2;
  }
  TokenInfo const& oper() const {
    return _oper;
  }

  ConditionOp(IOp* op1, IOp* op2, TokenInfo oper): _op1(op1), _op2(op2), _oper(oper) {
    setLastError(OK); 
    //check for errors:
    if (op1->getLastError() != OK) {
      setLastError(op1->getLastError());
      setErrorInfo(op1->getErrorInfo());
      return;
    }
    if (!(oper.type == EQ || 
          oper.type == NEQ ||
          oper.type == GEQ ||
          oper.type == LEQ ||
          oper.type == GREATER ||
          oper.type == LESS ) ) {
      setLastError(SYNTAX);
      setErrorInfo(ErrorInfo(oper.line));
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

class IfOp: public IOp {
  std::unique_ptr<IOp> _condition;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  std::unique_ptr<IOp> const & condition() const {
    return _condition;
  }
  std::vector<std::unique_ptr<IOp> > const & statements() const { 
    return _statements;
  }

  ~IfOp() { }

  IfOp(IOp* cond, std::vector<IOp* > statements): _condition(cond) { 
    setLastError(OK);
    //set up pointers ownership - to prevent memory loss:
    _statements.resize(statements.size());
    for (int i = 0; i < (int)statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
    //check for errors:
    if (cond->getLastError() != OK) {
      setLastError(cond->getLastError());
      setErrorInfo(cond->getErrorInfo());
      return;
    }
    for (int i = 0; i < (int)statements.size(); i ++) {
      if (statements[i]->getLastError() != OK) {
        setLastError(statements[i]->getLastError());
        setErrorInfo(statements[i]->getErrorInfo());
        break;
      }
    }
  }
  
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  void kickUpVars(std::set<std::string>* target) {
    _condition->kickUpVars(target);
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
      (*iter)->kickUpVars(target);
    } 
  }
};


class WhileOp: public IOp {
  std::unique_ptr<IOp> _condition;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  std::unique_ptr<IOp> const & condition() const {
    return _condition;
  }
  std::vector<std::unique_ptr<IOp> > const & statements() const { 
    return _statements;
  }

  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  ~WhileOp() { }
  WhileOp(IOp* cond, std::vector<IOp* > statements): _condition(cond) { 
    setLastError(OK);
    //set up pointers ownership - to prevent memory loss:
    _statements.resize(statements.size());
    for (int i = 0; i < (int)statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
    //check for errors:
    if (cond->getLastError() != OK) {
      setLastError(cond->getLastError());
      setErrorInfo(cond->getErrorInfo());
      return;
    }
    for (int i = 0; i < (int)statements.size(); i ++) {
      if (statements[i]->getLastError() != OK) {
        setLastError(statements[i]->getLastError());
        setErrorInfo(statements[i]->getErrorInfo());
        break;
      }
    }
  }

  void kickUpVars(std::set<std::string>* target) {
    _condition->kickUpVars(target);
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
      (*iter)->kickUpVars(target);
    } 
  }

};