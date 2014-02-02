#pragma once
#include "ivisitor.h"
#include "base.h"
#include "conditional.h"
#include "error_handling.h"
#include "functions.h"
#include "io.h"
#include "operations.h"
#include "context.h"

class Printer: public IVisitor{
  std::ostream* outputstream;
public:
  Printer(std::ostream* os) {
    outputstream = os; 
  }
  ~Printer() {}
  ResultInfo visit(BasicOp const* op);

  ResultInfo visit(ConditionOp const* op);
  ResultInfo visit(IfOp const* op);
  ResultInfo visit(WhileOp const* op);

  ResultInfo visit(ReadOp const* op);
  ResultInfo visit(PrintOp const* op);

  ResultInfo visit(PlusOp const* op);
  ResultInfo visit(MinusOp const* op);
  ResultInfo visit(MultOp const* op);
  ResultInfo visit(DivideOp const* op);
  ResultInfo visit(AssignOp const* op);

  ResultInfo visit(FunctionDefOp * op);
  ResultInfo visit(FunctionCallOp const* op);
  ResultInfo visit(ReturnOp const* op);
};