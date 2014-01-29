#include "context.h"

bool Context::hasVariable(std::string const& variable) {
  if (_variables_hash.find(variable) != _variables_hash.end()) 
    return true;
  if (_parent_context != NULL) {
    if (_parent_context->hasVariable(variable))
      return true;
  }
  return false;
}

void Context::setVariable(std::string const& variable, int value) {
  if (_parent_context != NULL) {
    if (_parent_context->hasVariable(variable)) {
      _parent_context->setVariable(variable, value);
      return;
    }
  }
  this->_variables_hash[variable] = value;
}


int Context::getValue(std::string const& variable) {
  std::map<std::string, int>::iterator iter_value = _variables_hash.find(variable);
  if (iter_value != _variables_hash.end()) {
    setLastError(ErrorType::OK);
    return (*iter_value).second;
  }
  if (_parent_context != NULL) {
    setLastError(_parent_context->getLastError());
    return _parent_context->getValue(variable);
  }
  setLastError(ErrorType::UNDEF_VARIABLE);
  return 0;
}

void Context::print() {
}