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
  FunctionDefOp(std::string function_name, std::vector<std::string> function_parameter_names, std::vector<IOp*> statements) { 
    this->setName(function_name);
    _param_names = function_parameter_names;
    _statements.resize(statements.size());
    for (int i = 0; i < statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
  }
  //after being called with specified values gets computed...
  //all params are being 
  //TODO: to write implementation
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    print();
    setLastError(OK);
    return 0;
  }

  virtual void print() {
    std::cout << "name:" << this->getName() << std::endl;
    std::cout << "params: ";
    for (std::vector<std::string>::iterator iter = _param_names.begin(); iter!=_param_names.end(); ++iter) {
      std::cout <<(*iter) << " ";
    }
    std::cout << std::endl;
    std::cout << "while statements: "<<std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->print();
    }
    
  }

  bool valid() {
    return true;
    //return  _condition->valid();
    //TODO: should validate contents too
  }
};

class FunctionCallOp: public IOp {
  std::string _name;
  std::vector<std::unique_ptr<IOp> > _statements;
public:
  FunctionCallOp(TokenInfo function_name, std::vector<IOp*> statements) { 
    _name = function_name.token;
   
    _statements.resize(statements.size());
    for (int i = 0; i < statements.size(); i ++) {
      _statements[i] = std::unique_ptr<IOp>(statements[i]);
    }
  }
  //after being called with specified values gets computed...
  //all params are being 
  //TODO: to write implementation
  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    
    print();
    setLastError(OK);
    return 0;
  }

  virtual void print() {
    std::cout << "function call:" << _name << std::endl;
    std::cout << "params: ";
    std::cout << "parameters: "<<std::endl;
    for (std::vector<std::unique_ptr<IOp> >::iterator iter = _statements.begin(); iter != _statements.end(); ++ iter) {
        (*iter)->print();
    }
    
  }

  bool valid() {
    return true;
    //return  _condition->valid();
    //TODO: should validate contents too
  }
};