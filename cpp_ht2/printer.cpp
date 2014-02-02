#include "printer.h"

ResultInfo Printer::visit(BasicOp const* op) {
  (*outputstream) << "BasicOp: " << op->get_value().token << std::endl;
  return ResultInfo(0, op->get_value().line);
}

ResultInfo Printer::visit(ConditionOp const* op) {
  (*outputstream) << "ConditionOp:" << std::endl;
  op->op1()->acceptVisitor(this);
  (*outputstream) << " condition op2:"<< std::endl;
  op->op2()->acceptVisitor(this);

  return ResultInfo(0, op->oper().line);
}

ResultInfo Printer::visit(IfOp const* op) {
  (*outputstream) << "If:" << std::endl;
  op->condition()->acceptVisitor(this);
  (*outputstream) << "if lines: "<< std::endl;
  for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++iter) {
    (*iter)->acceptVisitor(this);
  } 
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(WhileOp const* op) {
  (*outputstream) << "while:" << std::endl;
  op->condition()->acceptVisitor(this);
  (*outputstream) << "while statements: "<<std::endl;
  for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++ iter) {
    (*iter)->acceptVisitor(this);
  }
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(ReadOp const* op) {
  (*outputstream) << "ReadOp:" << op->variable().token << std::endl; 
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(PrintOp const* op) {
  (*outputstream) << "PrintOp:" << std::endl;
  op->value()->acceptVisitor(this); 
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(PlusOp const* op) {
  (*outputstream) << "PlusOp: " << std::endl;
  (*outputstream) << " PlusOp op1: " << std::endl;
  op->op1()->acceptVisitor(this);
  (*outputstream) << " PlusOp op2: " << std::endl;
  op->op2()->acceptVisitor(this);
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(MinusOp const* op)  {
  (*outputstream) << "MinusOp: " << std::endl;
  (*outputstream) << " MinusOp op1: " << std::endl;
  op->op1()->acceptVisitor(this);
  (*outputstream) << " MinusOp op2: " << std::endl;
  op->op2()->acceptVisitor(this);
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(MultOp const* op) {
  (*outputstream) << "MultOp: " << std::endl;
  (*outputstream) << " MultOp op1: " << std::endl;
  op->op1()->acceptVisitor(this);
  (*outputstream) << " MultOp op2: " << std::endl;
  op->op2()->acceptVisitor(this);
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(DivideOp const* op) {
  (*outputstream) << "DivideOp: " << std::endl;
  (*outputstream) << " DivideOp op1: " << std::endl;
  op->op1()->acceptVisitor(this);
  (*outputstream) << " DivideOp op2: " << std::endl;
  op->op2()->acceptVisitor(this);
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(AssignOp const* op) {
  (*outputstream) << "Assign: " << std::endl;
  (*outputstream) << " var name: " << op->variable().token << std::endl;
  (*outputstream) << " Assign op value: " << std::endl;
  op->value()->acceptVisitor(this);
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(FunctionDefOp * op) {
  (*outputstream) << "name:" << op->getName() << std::endl;
  (*outputstream) << "params: ";
  for (std::vector<std::string>::const_iterator iter = op->getParamNames().begin(); iter != op->getParamNames().end(); ++iter) {
    (*outputstream) << (*iter) << " ";
  }
  (*outputstream) << std::endl;
  (*outputstream) << "function statements: " << std::endl;
  for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++ iter) {
    (*iter)->acceptVisitor(this);
  }
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(FunctionCallOp const* op) {
  (*outputstream) << "function call:" << op->name() << std::endl;
  (*outputstream) << "params: ";
  (*outputstream) << "parameters: "<<std::endl;
  for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++ iter) {
    (*iter)->acceptVisitor(this);
  }
  return ResultInfo(0, 0);
}

ResultInfo Printer::visit(ReturnOp const* op) {
  (*outputstream) << "ReturnOp:" << std::endl;
  op->value()->acceptVisitor(this);    
  return ResultInfo(0, 0);
}