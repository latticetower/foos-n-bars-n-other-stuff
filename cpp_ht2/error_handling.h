#pragma once
#include <string>
#include "basic_types.h"
#include "context.h"
#include "base.h"


class InvalidOp: public IOp {

public: 
  ~InvalidOp() { }
  InvalidOp() { 
    setLastError(UNKNOWN); 
  }
  InvalidOp(ErrorType error, int line_no) {
    setLastError(error); 
    setErrorInfo(ErrorInfo(line_no));
  }

  ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    return ResultInfo(0);
  }

  void print(std::ostream& os) {
    os << "InvalidOp!" << std::endl;
  }
};