#pragma once
#include <string>
#include "basic_types.h"
#include "context.h"
#include "base.h"


class InvalidOp: public IOp {
  // std::string _info;
  ErrorInfo _info;
public: 
  InvalidOp(): _info() { setLastError(UNKNOWN); }
  InvalidOp(ErrorType error, int line_no): _info(error, line_no) { setLastError(error); }
  InvalidOp(ErrorType error, const char* error_info, int line_no): _info(error, line_no, error_info) { setLastError(error); }

  int Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    return 0;
  }

  const ErrorInfo getErrorInfo() {
    return ErrorInfo();
  }

  void setErrorInfo(ErrorInfo ei) {
    _info.line = ei.line;
    _info.location = std::string(ei.location);
  }

  bool valid() {
    return false;
  }
  void print() {
    std::cout << "InvalidOp!" << std::endl;
  }
};