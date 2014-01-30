#pragma once
#include <memory>
#include "basic_types.h"
#include "context.h"
#include "base.h"

//FunctionDefOp(function_name, function_parameters_names, sequence)

class FunctionDefOp: public IOp {
  std::vector<std::string> _param_names;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
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
  //after being called with specified values gets computed...
  //all params are being 
  //TODO: to write implementation
  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    print(std::cout);
    return 0;
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

};

class FunctionCallOp: public IOp {
  std::string _name;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  FunctionCallOp(TokenInfo function_name, std::vector<IOp*> statements) { 
    _name = function_name.token;
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
  //all params are being 
  //TODO: to write implementation
  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    print(std::cout);
    return ResultInfo(0, 0);
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
    std::cout << expr_result.result << std::endl;
    return expr_result;
  }

  void print(std::ostream& os) {
    os << "ReturnOp:" << std::endl;
    _value->print(os);    
  }
};