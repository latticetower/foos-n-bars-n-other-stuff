#pragma once
#include <memory>
#include "basic_types.h"
#include "context.h"
#include "base.h"

//FunctionDefOp(function_name, function_parameters_names, sequence)
/**
Object contains information about specific function. After called, method Compute returns function value.
there is also a local object cashe - to save memory when there is recursion 
*/
class FunctionDefOp: public IOp {
  std::vector<std::string> _param_names;
  std::vector<std::unique_ptr<IOp> > _statements;

  std::map<std::vector<int>, int> _call_cache;
public:
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
  }

  bool hasCachedValue(std::vector<int> values) {
    std::map<std::vector<int>, int>::iterator iter = _call_cache.find(values);
    return (iter != _call_cache.end());
  }

  ResultInfo getCachedValue(std::vector<int> values) {
    std::map<std::vector<int>, int>::iterator iter = _call_cache.find(values);
    if (iter != _call_cache.end()) {
      return ResultInfo(iter->second, 0, OK);
    }
    return ResultInfo();
  }

  void setCachedValue(std::vector<int> input_values, ResultInfo func_value) {
    if (func_value.error_type() == OK)
      _call_cache[input_values] = func_value.result;
  }

  //after being called with specified values gets computed...
  //all params are being 
  //TODO: to write implementation
  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    // first check up if there are some values computed previously
    std::vector<int> values;
    for (std::vector<std::string>::iterator iter = _param_names.begin(); iter != _param_names.end(); ++ iter) {
      values.push_back(context->getValue(*iter));
    }
    
    if (hasCachedValue(values)) {
      return getCachedValue(values);
    }

    // if not, try to compute:
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++iter) {
      ResultInfo ri = (*iter)->Compute(context, _functions);
      if (ri.error_type() == FUNCTION_RETURN) {
        ResultInfo ri2(ri.result, ri.error_info.line, OK);
        setCachedValue(values, ri2);
        return ri2;
      }
      if (ri.error_type() != OK) {
        setCachedValue(values, ri);
        return ri;
      }
    }
    // print(std::cout);
    ResultInfo ri(0, 0, OK, getName());
    setCachedValue(values, ri);
    return ri;
  }

  virtual void print(std::ostream& os) {
    os << "name:" << this->getName() << std::endl;
    os << "params: ";
    for (std::vector<std::string>::iterator iter = _param_names.begin(); iter!=_param_names.end(); ++iter) {
      os << (*iter) << " ";
    }
    os << std::endl;
    os << "while statements: "<<std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->print(os);
    }
    
  }

  std::vector<std::string> const & getParamNames() {
    return _param_names;
  }

};

class FunctionCallOp: public IOp {
  std::string _name;
  int _line;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
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

  //after being called with specified values gets computed...
  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    std::map<std::string, std::unique_ptr<IOp> >::const_iterator pToFunction = _functions.find(_name);
    if (pToFunction == _functions.end()) {
      return ResultInfo(0, _line, UNDEF_FUNCTION, _name);
    }
    
       
    FunctionDefOp* functionDefPointer = dynamic_cast<FunctionDefOp*>(pToFunction->second.get());
    std::vector<std::string> param_names = functionDefPointer->getParamNames();
    if (param_names.size() != _statements.size()) {
      return ResultInfo(0, _line, ARGS_MISMATCH, _name);
    }
    
    std::vector<int> values;
    for (int i = 0; i < (int)param_names.size(); i++) {
      ResultInfo ri = _statements[i]->Compute(context, _functions);
      if (ri.error_type() != OK)
        return ri;
      values.push_back(ri.result);
    }
    if (functionDefPointer->hasCachedValue(values)) {
      return functionDefPointer->getCachedValue(values);
    }
    
    Context func_context(context); // give context to allow use of global variables in function

    for (int i = 0; i < (int)param_names.size(); i++) {
      func_context.setVariable(param_names[i], values[i]);
    }
    return functionDefPointer->Compute(&func_context, _functions);
  }

  virtual void print(std::ostream& os) {
    os << "function call:" << _name << std::endl;
    os << "params: ";
    os << "parameters: "<<std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->print(os);
    }
    
  }
};


class ReturnOp: public IOp {
  std::unique_ptr<IOp> _value;
public:
  ~ReturnOp() { }

  ReturnOp() {}
  ReturnOp(IOp* op): _value(op) { 
    setLastError(op->getLastError()); 
    setErrorInfo(op->getErrorInfo());
  }
  
  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    ResultInfo expr_result = _value->Compute(context, _functions);
    if (expr_result.error_type() != OK) {
      return expr_result;
    }
    expr_result.error_info.type = FUNCTION_RETURN;
   
    return expr_result;
  }

  void print(std::ostream& os) {
    os << "ReturnOp:" << std::endl;
    _value->print(os);    
  }
};