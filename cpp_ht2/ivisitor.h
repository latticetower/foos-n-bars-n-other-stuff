#pragma once
#include "basic_types.h"

class BasicOp;
class ConditionOp;
class IfOp;
class WhileOp;

class ReadOp;
class PrintOp;

class PlusOp;
class MinusOp;
class MultOp;
class DivideOp;
class AssignOp;

class FunctionDefOp;
class FunctionCallOp;
class ReturnOp;

class IVisitor{
public:
  virtual ~IVisitor() {}
  virtual ResultInfo visit(BasicOp const* op) = 0;
  virtual ResultInfo visit(ConditionOp const* op) = 0;
  virtual ResultInfo visit(IfOp const* op) = 0;
  virtual ResultInfo visit(WhileOp const* op) = 0;

  virtual ResultInfo visit(ReadOp const* op) = 0;
  virtual ResultInfo visit(PrintOp const* op) = 0;

  virtual ResultInfo visit(PlusOp const* op) = 0;
  virtual ResultInfo visit(MinusOp const* op) = 0;
  virtual ResultInfo visit(MultOp const* op) = 0;
  virtual ResultInfo visit(DivideOp const* op) = 0;

  virtual ResultInfo visit(AssignOp const* op) = 0;

  virtual ResultInfo visit(FunctionDefOp * op) = 0;
  virtual ResultInfo visit(FunctionCallOp const* op) = 0;
  virtual ResultInfo visit(ReturnOp const* op) = 0;
};