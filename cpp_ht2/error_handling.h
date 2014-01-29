#pragma once
#include <string>
#include "basic_types.h"
#include "context.h"
#include "base.h"


class InvalidOp: public IOp {
  std::string _info;
public: 
  InvalidOp(): _info("") {}
  InvalidOp(ErrorType error): _info("") { setLastError(error); }
  InvalidOp(ErrorType error, const char* error_info): _info(error_info) { setLastError(error); }
  int Compute(IContext* context) {
    return 0;
  }

  bool valid() {
    return false;
  }
  void print() {
    std::cout << "InvalidOp!" << std::endl;
  }
};