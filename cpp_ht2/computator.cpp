#include "computator.h"

ResultInfo Computator::visit(BasicOp const* op) {
  if (op->get_value().type == NUMBER) {
    return ResultInfo(atoi(op->get_value().token.c_str()), op->get_value().line);
  } 
  else {
    if (op->get_value().type == VAR) {
      if (context != NULL) {
        int vi = context->getValue(op->get_value().token);
        return ResultInfo(vi, op->get_value().line, context->getLastError(), op->get_value().token);
      }
    }
  }
  return ResultInfo(0, op->get_value().line, UNDEF_VARIABLE, op->get_value().token);
}

ResultInfo Computator::visit(ConditionOp const* op) {
  ResultInfo op1_value = op->op1()->acceptVisitor(this);
  ResultInfo op2_value = op->op2()->acceptVisitor(this);
  
  if (op->oper().type == EQ)
    return op1_value == op2_value;
  if (op->oper().type == NEQ)
    return op1_value != op2_value;
  if (op->oper().type == GREATER)
    return op1_value > op2_value;
  if (op->oper().type == GEQ)
    return op1_value >= op2_value;
  if (op->oper().type == LESS)
    return op1_value < op2_value;
  if (op->oper().type == LEQ)
    return op1_value <= op2_value;

  return ResultInfo(0, op->oper().line, UNKNOWN);
}

ResultInfo Computator::visit(IfOp const* op) {
 // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  ResultInfo condition_result = op->condition()->acceptVisitor(this);
  if (condition_result.result) {
    for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++iter) {
      ResultInfo statement_result = (*iter)->acceptVisitor(this);
      if (statement_result.error_type() != OK) {
        return statement_result;
      }
    }
  }
  return ResultInfo(0, condition_result.error_info.line);
 // }
}

ResultInfo Computator::visit(WhileOp const* op) {
//ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  while (op->condition()->acceptVisitor(this).result) {
    for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++ iter) {
      ResultInfo result = (*iter)->acceptVisitor(this);
      if (result.error_type() != OK) {
        return result;
      }
    }
  }
  return ResultInfo(0, 0);
  //}
}

ResultInfo Computator::visit(ReadOp const* op) {
  // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    if (context == NULL) {
      return ResultInfo(0, op->variable().line, NO_CONTEXT);
    }
    if (!context->hasVariable(op->variable().token)) {
      return ResultInfo(0, op->variable().line, UNDEF_VARIABLE, op->variable().token);
    }
    int expr_result = 0;
    std::cin >> expr_result;

    context->setVariable(op->variable().token, expr_result);
    return ResultInfo(expr_result, op->variable().line);
  //}
}

ResultInfo Computator::visit(PrintOp const* op) {
  // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  ResultInfo expr_result = op->value()->acceptVisitor(this);
  if (expr_result.error_type() != OK) {
    return expr_result;
  }
  std::cout << expr_result.result << std::endl;
  return expr_result;
 // }
}

ResultInfo Computator::visit(PlusOp const* op) {
 // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  ResultInfo op1_value = op->op1()->acceptVisitor(this);
  ResultInfo op2_value = op->op2()->acceptVisitor(this);
  return op1_value + op2_value;
  //}
}

ResultInfo Computator::visit(MinusOp const* op)  {
  ResultInfo op1_value = op->op1()->acceptVisitor(this);
  ResultInfo op2_value = op->op2()->acceptVisitor(this);
  return op1_value - op2_value;
}

ResultInfo Computator::visit(MultOp const* op) {
  ResultInfo op1_value = op->op1()->acceptVisitor(this);
  ResultInfo op2_value = op->op2()->acceptVisitor(this);
  return op1_value * op2_value;
}

ResultInfo Computator::visit(DivideOp const* op) {
  ResultInfo op1_value = op->op1()->acceptVisitor(this);
  ResultInfo op2_value = op->op2()->acceptVisitor(this);
  return op1_value / op2_value;
}

ResultInfo Computator::visit(AssignOp const* op) {
  if (context == NULL) {
    return ResultInfo(0, op->variable().line, NO_CONTEXT);
  }
  ResultInfo var_value = op->value()->acceptVisitor(this);
  if (var_value.error_type() != OK) {
    return var_value;
  }
    
  context->setVariable(op->variable().token, var_value.result);
  return var_value;
}

ResultInfo Computator::visit(FunctionDefOp * op) {

  // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    // first check up if there are some values computed previously
    std::vector<int> actual_var_values = op->getActualVarValues(context);
    
    if (op->hasCachedValue(actual_var_values)) {
      return op->getCachedValue(actual_var_values);
    }

    // if not, try to compute:
    for (std::vector<std::unique_ptr<IOp> >::const_iterator iter = op->statements().begin(); iter != op->statements().end(); ++ iter) {
      ResultInfo ri = (*iter)->acceptVisitor(this);
      if (ri.error_type() == FUNCTION_RETURN) {
        ResultInfo ri2(ri.result, ri.error_info.line, OK);
        op->setCachedValue(actual_var_values, ri2);
        return ri2;
      }
      if (ri.error_type() != OK) {
        op->setCachedValue(actual_var_values, ri);
        return ri;
      }
    }
    // print(std::cout);
    ResultInfo ri(0, 0, OK, op->getName());
    op->setCachedValue(actual_var_values, ri);
    return ri;
}
ResultInfo Computator::visit(FunctionCallOp const* op) {
 // ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
    std::map<std::string, std::unique_ptr<IOp> >::iterator pToFunction = functions->find(op->name());
    if (pToFunction == functions->end()) {
      return ResultInfo(0, op->line(), UNDEF_FUNCTION, op->name());
    }
    
    FunctionDefOp* functionDefPointer = dynamic_cast<FunctionDefOp*>(pToFunction->second.get());
    std::vector<std::string> param_names = functionDefPointer->getParamNames();
    if (param_names.size() != op->statements().size()) {
      return ResultInfo(0, op->line(), ARGS_MISMATCH, op->name());
    }
    
    std::vector<int> values;
    for (int i = 0; i < (int)param_names.size(); i++) {
      ResultInfo ri = op->statements()[i]->acceptVisitor(this);
      if (ri.error_type() != OK)
        return ri;
      values.push_back(ri.result);
    }

    std::vector<int> actual_values = functionDefPointer->getActualVarValues(context);

    if (functionDefPointer->hasCachedValue(actual_values)) {
      return functionDefPointer->getCachedValue(actual_values);
    }
    
    Context func_context(context); // give context to allow use of global variables in function

    for (int i = 0; i < (int)param_names.size(); i++) {
      func_context.setVariable(param_names[i], values[i]);
    }
    Computator func_computator(&func_context, functions);
    return functionDefPointer->acceptVisitor(&func_computator);
  //}
}
ResultInfo Computator::visit(ReturnOp const* op) {
   //ResultInfo Compute(IContext* context, std::map<std::string, std::unique_ptr<IOp> > const & _functions) {
  ResultInfo expr_result = op->value()->acceptVisitor(this);
  if (expr_result.error_type() != OK) {
    return expr_result;
  }
  expr_result.error_info.type = FUNCTION_RETURN;
   
  return expr_result;
  //}
}