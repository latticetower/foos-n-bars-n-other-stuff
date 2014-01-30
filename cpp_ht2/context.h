#pragma once
#include <map>
#include <string>
#include "basic_types.h"

class IContext{
  ErrorType _error;
protected:
  void setLastError(ErrorType error) {
    _error = error;
  }
public:
  IContext() {
    setLastError(UNKNOWN);
  }
  ErrorType getLastError() {
    return _error;
  }

  virtual void setVariable(std::string const& variable, int value) = 0;
  
  //checks if variable present in context hash
  virtual bool hasVariable(std::string const& variable) = 0;
  
  //returns value with error code
  virtual int getValue(std::string const& variable) = 0;
};

//class translates some additional information to operations
//contains hash of variables and pointers to functions

class Context: public IContext {
  IContext * _parent_context;
  std::map<std::string, int> _variables_hash;
public:
  Context(IContext* parent = NULL) { 
    _parent_context = parent;
    setLastError(OK);
  }
  
  //adds variable to context hash
  void setVariable(std::string const& variable, int value);
  
  //checks if variable present in context hash
  bool hasVariable(std::string const& variable);
  
  //returns value with error code
  int getValue(std::string const& variable);

  //debug method. prints to some output stream information about current context state
  void print();
};

