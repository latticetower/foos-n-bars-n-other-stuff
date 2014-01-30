#include "parser.h"

IOp* Parser::getRead(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == READ) {
    lexer->getNextToken();
    TokenInfo var = lexer->getNextToken(); //TODO:add check
    if (var.type != VAR) {
      return new InvalidOp(SYNTAX, var.line);
    }
    return new ReadOp(var);
  }
  return new InvalidOp(UNKNOWN, var.line);
}

IOp* Parser::getPrint(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == PRINT) {
    lexer->getNextToken();
    IOp* op1 = getExpr(lexer);
    if (op1->getLastError() != OK) {
      return op1;//there is no point to construct new object
    }
    return new PrintOp(op1);
  }
  return new InvalidOp(UNKNOWN, 0);
}

IOp* Parser::getReturn(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == RETURN) {
    lexer->getNextToken();
    IOp* op1 = getExpr(lexer);
    if (op1->getLastError() != OK) {
      return op1;
    }
    return new ReturnOp(op1);
  }
  return new InvalidOp(UNKNOWN, var.line);
}

IOp* Parser::getAssignmentOrFunctionCall(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == VAR) {
    lexer->getNextToken();
    TokenInfo next = lexer->peekNextToken();
    if (next.type == ASSIGN) {
      return getAssignment(lexer, var);
    }
    else{
      if (next.type == LBRACKET) {
        return getFunctionCall(lexer, var);
      }
      else {
        //something strange in the neighbourhood
        return new InvalidOp(SYNTAX, next.line);
      }
    }
  }
  return new InvalidOp(SYNTAX, var.line);
}

// =
IOp* Parser::getAssignment(Lexer* lexer, TokenInfo var) {
  if (var.type == VAR) {
    TokenInfo next = lexer->peekNextToken();
    if (next.type == ASSIGN) {
      lexer->getNextToken();
      IOp* op1 = getExpr(lexer);
      if (op1->getLastError() != OK) {
        return op1;
      }
      return new AssignOp(var, op1);
    }
  }
  return new InvalidOp(SYNTAX, var.line);
}


IOp* Parser::getExpr(Lexer* lexer) {
  //return getExprPrior2(l);
  return getExprPrior1(lexer);
}


// +, -
IOp* Parser::getExprPrior1(Lexer* lexer) {
  //check for unary -:
  TokenInfo ti = lexer->peekNextToken();
  IOp* op1 = NULL;
  if (ti.type == MINUS){
    op1 = new BasicOp(TokenInfo(TokenType::NUMBER, "0", ti.line));
  }
  else {
    op1 = getExprPrior2(lexer);
  }
  if (op1->getLastError() != OK)
    return op1;//TODO: add condition for return statement too
  //next, we process the following operands:
  TokenInfo current_operation = lexer->peekNextToken();
  while (current_operation.type  == PLUS || current_operation.type == MINUS) {
    lexer->getNextToken();
    IOp* op2 = getExprPrior2(lexer);
    if (current_operation.type == PLUS) {
      op1 = new PlusOp(op1, op2);
      
    }
    if (current_operation.type == MINUS) {
      op1 = new MinusOp(op1, op2);
    }
    //op1->getLastError() call will return != OK value if there are errors
    if (op1->getLastError() != OK)
      return op1;// to prevent memory loss, we return all we managed to get from lexer and instantiated with new

    current_operation = lexer->peekNextToken();
    if (current_operation.type == ENDOFFILE)
      break;
  }
  return op1;
}

//  *, /
IOp* Parser::getExprPrior2(Lexer* lexer) {
  // 1. get first operand
  IOp* op1 = getExprPrior3(lexer);
  if (op1->getLastError() != OK) {
    return op1;
  }
  TokenInfo current_operation = lexer->peekNextToken();
  while (current_operation.type == MULT || current_operation.type == DIV) {
    lexer->getNextToken();
    IOp* op2 = getExprPrior3(lexer);
    if (current_operation.type == MULT) {
      op1 = new MultOp(op1, op2);
    }
    if (current_operation.type == DIV) {
      op1 = new DivideOp(op1, op2);
    }

    if (op1->getLastError() != OK)
      return op1;// to prevent memory loss, we return all we managed to get from lexer and instantiated with new

    current_operation = lexer->peekNextToken();
    if (current_operation.type == ENDOFFILE)
      break;
  }
  return op1;

}

//  (
IOp* Parser::getExprPrior3(Lexer* lexer) {
  IOp* result;
  TokenInfo t = lexer->peekNextToken();
  if (t.type == LBRACKET) {
    lexer->getNextToken();
    result = getExprPrior1(lexer);
    lexer->getNextToken();//TODO: add check for rbracket
  }
  else {
    if (t.type != ENDOFFILE) {
      TokenInfo var = lexer->getNextToken();
      TokenInfo next = lexer ->peekNextToken();
      if (var.type == VAR) {
        if (next.type == LBRACKET) {
          result = getFunctionCall(lexer, var);
        }
        else result = new BasicOp(var);
      }
      else {
        if (var.type == NUMBER)
          result = new BasicOp(var);
        else
        result = new InvalidOp(SYNTAX, var.line);
      }
    }
    else {
      result = new InvalidOp(SYNTAX, t.line);
    }
  }
  
  return result;
}

///conditions

IOp* Parser::getCondition(Lexer* lexer) {
  IOp* op1 = getExpr(lexer);
  if (op1->getLastError() != OK)
      return op1;

  TokenInfo var = lexer->peekNextToken();
  if (var.type == EQ ||
      var.type == NEQ || 
      var.type == GREATER ||
      var.type == GEQ ||
      var.type == LESS ||
      var.type == LEQ) {

    lexer->getNextToken();
    IOp* op2 = getExpr(lexer);
    //if there is error in op2, op1 and op2 are get collected in ConditionOp 
    return new ConditionOp(op1, op2, var);
  }
  delete op1;
  return new InvalidOp(SYNTAX, var.line);
}


IOp* Parser::getIf(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == IF) {
    lexer->getNextToken();
    IOp* condition = getCondition(lexer);
    if (condition->getLastError() != OK) {
      return condition;
    }
    var = lexer->getNextToken();
    if (var.type != COLON) {
      delete condition;
      return new InvalidOp(SYNTAX, var.line);
    }
    
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    if (var.type != END) {
      delete condition;
      for (int i = 0; i < (int)sequence.size(); i++) {
        delete sequence[i];
      }
      return new InvalidOp(SYNTAX, var.line);
    }
    return new IfOp(condition, sequence);
  }
  return new InvalidOp(UNKNOWN, var.line);
}


IOp* Parser::getWhile(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == WHILE) {
    lexer->getNextToken();
    IOp* condition = getCondition(lexer);
    if (condition->getLastError() != OK) {
      return condition;
    }
    var = lexer->getNextToken();
    if (var.type != COLON) {
      delete condition;
      //report error
      return new InvalidOp(SYNTAX, var.line);
    }
    
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    if (var.type != END) {
      delete condition;
      for (int i = 0; i < (int)sequence.size(); i++)
        delete sequence[i];
      return new InvalidOp(SYNTAX, var.line);
    }
    return new WhileOp(condition, sequence);
  }
  return new InvalidOp(UNKNOWN, var.line);
}

IOp* Parser::getFunctionDef(Lexer*lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == DEF) {
    lexer->getNextToken();
    TokenInfo function_name = lexer->getNextToken();
    if (function_name.type != VAR) {
      return new InvalidOp(SYNTAX, function_name.line);
    }
    var = lexer->getNextToken();
    if (var.type != LBRACKET) {
      return new InvalidOp(SYNTAX, var.line);
    }
    std::vector<std::string> function_parameters_names;
    var = lexer->getNextToken();
    while (var.type != RBRACKET && var.type != ENDOFFILE) {
      if (var.type != VAR) {
        return new InvalidOp(SYNTAX, var.line);
      }
      function_parameters_names.push_back(var.token);
      var = lexer->getNextToken();
    }
    var = lexer->getNextToken();//COLON
    if (var.type != COLON) {
      return new InvalidOp(SYNTAX, var.line);
    }
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    if (var.type != END) {
      for (int i = 0; i < (int)sequence.size(); i++)
        delete sequence[i];
      return new InvalidOp(SYNTAX, var.line);
    }
    return new FunctionDefOp(function_name.token, function_parameters_names, sequence);
  }
  return new InvalidOp(SYNTAX, var.line);
}

IOp* Parser::getFunctionCall(Lexer* lexer, TokenInfo var) {
  TokenInfo next = lexer->peekNextToken();
  if (next.type == LBRACKET) {
    lexer->getNextToken();
    
    std::vector<IOp* > function_parameters;
    next = lexer->peekNextToken();
    while (next.type != RBRACKET && next.type != ENDOFFILE) {
      IOp* param = this->getExpr(lexer);
      if (param->getLastError() != OK) {
        for (int i = 0; i < (int)function_parameters.size(); i++)
          delete function_parameters[i];
        return param;
      }
      function_parameters.push_back(param);
      var = lexer->peekNextToken();
    }

    next = lexer->getNextToken();// RBRACKET
    if (next.type != RBRACKET) {
      for (int i = 0; i < (int)function_parameters.size(); i++)
        delete function_parameters[i];
      return new InvalidOp(SYNTAX, next.line);
    }
    //TODO: check for RBRACKET and return syntax error
    return new FunctionCallOp(var, function_parameters);
  }
  return new InvalidOp(SYNTAX, next.line);
}

IOp* Parser::getNextExpression(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  
  if (ti.type == VAR) {
    return getAssignmentOrFunctionCall(lexer);
  }
  if (ti.type == PRINT) {
    return getPrint(lexer);
  }
  if (ti.type == READ) {
    return getRead(lexer);
  }
  if (ti.type == RETURN) {
    return getReturn(lexer);
  }
  if (ti.type == IF) {
    return getIf(lexer);
  }
  if (ti.type == WHILE) {
    return getWhile(lexer);
  }
  if (ti.type == DEF) {
    IOp* op = getFunctionDef(lexer);
    if (op->getLastError() != OK) {
      return op;
    }
    _functions[op->getName()] = std::unique_ptr<IOp>(op);
    return new BasicOp(TokenInfo(NUMBER, "0", ti.line));
  }
  return new InvalidOp(SYNTAX, ti.line);
}


std::vector<IOp* > Parser::getExpressionsSequence(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  std::vector<IOp* > expr;

  while (ti.type != ENDOFFILE && ti.type != END) {
    IOp* op = getNextExpression(lexer);
    expr.push_back(op);
    if (op->getLastError() != OK) {
      //clear vector
      //add only member - current error
      op->getErrorInfo();
      break;
    }
    ti = lexer->peekNextToken();
  }
  return expr;
}

void Parser::ComputeAll(Context* context) {
  for (std::vector<std::unique_ptr<IOp> >::iterator iter = _expressions.begin(); iter != _expressions.end(); ++iter) {
    ResultInfo computation_result = (*iter)->Compute(context, _functions);
    if (computation_result.error_type() != OK) {
      printErrorMessage(computation_result);
      return;
    }
  }
}

void Parser::printErrorMessage(ResultInfo ei) {
  if (ei.error_type() == OK)
    return;
  std::cout << "line " << ei.error_info.line << ": ";

  if (ei.error_type() == UNKNOWN)
    std::cout << "unknown error occured.";
  if (ei.error_type() == UNDEF_VARIABLE)
    std::cout << "undefined variable " << ei.error_info.location << ".";
  if (ei.error_type() == UNDEF_FUNCTION)
    std::cout << "undefined function " << ei.error_info.location << ".";
  if (ei.error_type() == SYNTAX)
    std::cout << "syntax error.";
  if (ei.error_type() == NO_CONTEXT)
    std::cout << "context not found...";
  if (ei.error_type() == FUNCTION_RETURN)
    std::cout << "function return statement occured.";
  if (ei.error_type() == DIVISION_BY_ZERO)
    std::cout << "division by zero.";
  if (ei.error_type() == ARGS_MISMATCH)
    std::cout << "arguments number mistmatch for "<< ei.error_info.location << ".";
      
  std::cout << std::endl;
}