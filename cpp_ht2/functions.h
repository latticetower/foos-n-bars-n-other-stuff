#pragma once
#include <memory>
#include "basic_types.h"
#include "context.h"
#include "base.h"
#include "ivisitor.h"

//FunctionDefOp(function_name, function_parameters_names, sequence)
/**
Object contains information about specific function. After called, method Compute returns function value.
there is also a local object cashe - to save memory when there is recursion 
*/
class FunctionDefOp: public IOp {
  std::vector<std::string> _param_names;
  std::vector<std::unique_ptr<IOp> > _statements;

  std::map<std::vector<int>, int> _call_cache;
  std::set<std::string> _actual_var_names;
public:
  std::vector<std::unique_ptr<IOp> > const & statements() const {
    return _statements;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }
  ~FunctionDefOp() { }

  FunctionDefOp(std::string function_name, std::vector<std::string> function_parameter_names, 
                std::vector<IOp*> statements) { 

    this->setName(function_name);
    _param_names = function_parameter_names;
    //set up pointers ownership - to prevent memory loss:
    _statements.resize(statements.size());
    for (int i = 0; i < (int)statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }

    //check for errors:
    for (int i = 0; i < (int)statements.size(); i ++) {
      if (statements[i]->getLastError() != OK) {
        setLastError(statements[i]->getLastError());
        setErrorInfo(statements[i]->getErrorInfo());
        return;
      }
    }
    //if everything is ok, parse and save to set actual variables
    for (int i = 0; i < (int)statements.size(); i ++) {
      _statements[i]->kickUpVars(&_actual_var_names);
    }

  }

  bool hasCachedValue(std::vector<int> & values) const {
    std::map<std::vector<int>, int>::const_iterator iter = _call_cache.find(values);
    return (iter != _call_cache.end());
  }

  ResultInfo const getCachedValue(std::vector<int> values) const{
    std::map<std::vector<int>, int>::const_iterator iter = _call_cache.find(values);
    if (iter != _call_cache.end()) {
      return ResultInfo(iter->second, 0, OK);
    }
    return ResultInfo();
  }

  void setCachedValue(std::vector<int> input_values, ResultInfo func_value) {
    if (func_value.error_type() == OK) {
      _call_cache[input_values] = func_value.result;
    }
  }

  std::set<std::string> const & getActualVarNames() const {
    return _actual_var_names;
  }

  std::vector<int>  getActualVarValues(IContext* context) {
    std::vector<int> actual_values;
    if (context == NULL)
      return actual_values;

    for (std::set<std::string>::iterator iter = _actual_var_names.begin(); iter!= _actual_var_names.end(); ++iter) {
      if (context->hasVariable(*iter))
        actual_values.push_back(context->getValue(*iter));
      else 
        actual_values.push_back(0);
    }
    return actual_values;
  }

  std::vector<std::string> const & getParamNames() {
    return _param_names;
  }

  void kickUpVars(std::set<std::string>* target) {
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
      (*iter)->kickUpVars(target);
    }
  }

};

class FunctionCallOp: public IOp {
  std::string _name;
  int _line;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  std::string const & name() const {
    return _name;
  }
  int const & line() const {
    return _line;
  }
  std::vector<std::unique_ptr<IOp> > const & statements() const {
    return _statements;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  ~FunctionCallOp() { }

  FunctionCallOp(TokenInfo function_name, std::vector<IOp*> statements) { 
    _name = function_name.token;
    _line = function_name.line;
    //set up pointers ownership - to prevent memory loss:
    _statements.resize(statements.size());
    for (int i = 0; i < (int)statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
    //check for errors:
    if (function_name.type != VAR) {
      setLastError(SYNTAX);
      setErrorInfo(ErrorInfo(function_name.line));
      return;
    }
    for (int i = 0; i < (int)statements.size(); i ++) {
      if (statements[i]->getLastError() != OK) {
        setLastError(statements[i]->getLastError());
        setErrorInfo(statements[i]->getErrorInfo());
        return;
      }
    }
  }

  void kickUpVars(std::set<std::string>* target) {
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
      (*iter)->kickUpVars(target);
    } 
  }

};


class ReturnOp: public IOp {
  std::unique_ptr<IOp> _value;
public:
  std::unique_ptr<IOp> const & value() const {
    return _value;
  }
  ResultInfo acceptVisitor(IVisitor * visitor) {
    return visitor->visit(this);
  }

  ~ReturnOp() { }
  ReturnOp() {}
  ReturnOp(IOp* op): _value(op) { 
    setLastError(op->getLastError()); 
    setErrorInfo(op->getErrorInfo());
  }

  void kickUpVars(std::set<std::string>* target) {
    _value->kickUpVars(target);
  }
};