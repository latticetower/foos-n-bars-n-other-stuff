#include "parser.h"

IOp* Parser::getRead(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == READ) {
    lexer->getNextToken();
    TokenInfo var = lexer->getNextToken(); //TODO:add check
    IOp* op1 = new ReadOp(var);
    return op1;
  }
  return new InvalidOp(UNKNOWN, 0);
}

IOp* Parser::getPrint(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == PRINT) {
    lexer->getNextToken();
    IOp* op1 = getExpr(lexer);
    op1 = new PrintOp(op1);
    return op1;
  }
  return new InvalidOp(UNKNOWN, 0);
}
IOp* Parser::getAssignmentOrFunctionCall(Lexer*lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == VAR) {
    lexer->getNextToken();
    TokenInfo next = lexer->peekNextToken();
    if (next.type == ASSIGN){
      return getAssignment(lexer, var);
    }
    else{
      if (next.type == LBRACKET) {
        return getFunctionCall(lexer, var);
      }
    }
    
  }
  return new InvalidOp(SYNTAX, 0);
}
// =
IOp* Parser::getAssignment(Lexer* lexer, TokenInfo var) {
  //TokenInfo var = lexer->peekNextToken();
  if (var.type == VAR) {
    //lexer->getNextToken();
    if (lexer->peekNextToken().type == ASSIGN) {
      lexer->getNextToken();
      IOp* op1 = getExpr(lexer);
      op1 = new AssignOp(var, op1);
      return op1;
    }
  }
  return new InvalidOp(SYNTAX, 0);
}


IOp* Parser::getExpr(Lexer* lexer) {
  //return getExprPrior2(l);
  return getExprPrior1(lexer);
}


// +, -
IOp* Parser::getExprPrior1(Lexer* lexer) {
  //check for unary -
  TokenInfo ti = lexer->peekNextToken();
  IOp* op1 = NULL;
  if (ti.type == MINUS){
    op1 = new BasicOp(TokenInfo(TokenType::NUMBER, "0", ti.line));
  }
  else {
    op1 = getExprPrior2(lexer);
  }
  if (op1->getLastError() != OK)//TODO: add condition for return statement too
    return op1;
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
  if (op1->getLastError() != OK) {//TODO: add some check for return value too
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
    current_operation = lexer->peekNextToken();
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
      if (var.type == VAR && next.type == LBRACKET) {
        result = getFunctionCall(lexer, var);
      }
      else result = new BasicOp(var);
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
  TokenInfo var = lexer->peekNextToken();
  if (var.type == EQ ||
      var.type == NEQ || 
      var.type == GREATER ||
      var.type == GEQ ||
      var.type == LESS ||
      var.type == LEQ) {

    lexer->getNextToken();
    IOp* op2 = getExpr(lexer);
    op1 = new ConditionOp(op1, op2, var);
    return op1;
  }
  return new InvalidOp(SYNTAX, var.line);
}


IOp* Parser::getIf(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == IF) {
    lexer->getNextToken();
    IOp* cond = getCondition(lexer);
    var = lexer->getNextToken();
    if (var.type != COLON) {
      //report error
      return new InvalidOp(SYNTAX, var.line);
    }
    
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    return new IfOp(cond, sequence);
  }
  return new InvalidOp(UNKNOWN, var.line);
}


IOp* Parser::getWhile(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == WHILE) {
    lexer->getNextToken();
    IOp* cond = getCondition(lexer);
    var = lexer->getNextToken();
    if (var.type != COLON) {
      //report error
      return new InvalidOp(SYNTAX, var.line);
    }
    
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    return new WhileOp(cond, sequence);
  }
  return new InvalidOp(UNKNOWN, var.line);
}

IOp* Parser::getFunctionDef(Lexer*lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.type == DEF) {
    lexer->getNextToken();
    TokenInfo function_name = lexer->getNextToken();

    var = lexer->getNextToken();
    if (var.type != LBRACKET) {
      //report error
      return new InvalidOp(SYNTAX, var.line);
    }
    std::vector<std::string> function_parameters_names;
    var = lexer->getNextToken();
    while (var.type != RBRACKET && var.type != ENDOFFILE) {
      function_parameters_names.push_back(var.token);
      var = lexer->getNextToken();
    }
    var = lexer->getNextToken();//COLON
    if (var.type != COLON) {
      return new InvalidOp(SYNTAX, var.line);
    }
    std::vector<IOp* > sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    return new FunctionDefOp(function_name.token, function_parameters_names, sequence);
  }
  return new InvalidOp(SYNTAX, var.line);
}

IOp* Parser::getFunctionCall(Lexer* lexer, TokenInfo var) {
  TokenInfo next = lexer->peekNextToken();
  if (next.type == LBRACKET) {
    lexer->getNextToken();
    
    //if (var.type != LBRACKET) {
      //report error
     // return new InvalidOp(SYNTAX, var.line);
    //}
    std::vector<IOp* > function_parameters;
    next = lexer->peekNextToken();
    while (next.type != RBRACKET && next.type != ENDOFFILE) {
      IOp* param = this->getExpr(lexer);
      function_parameters.push_back(param);
      var = lexer->peekNextToken();
    }

    next = lexer->getNextToken();//END
    //TODO: check for RBRACKET and return syntax error
    return new FunctionCallOp(var, function_parameters);
  }
  return new InvalidOp(SYNTAX, 0);
}

IOp* Parser::getNextExpression(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  IOp* op;
  if (ti.type == VAR) {
    op = getAssignmentOrFunctionCall(lexer);
    return op;
    //op->print();
    //std::cout << "-------------------" << std::endl;   
  }
  if (ti.type == PRINT) {
    op = getPrint(lexer);
    return op;
  //  op->print();
  //  std::cout << "-------------------" << std::endl;
  }
  if (ti.type == READ) {
    op = getRead(lexer);
    return op;
    //op->print();
    //std::cout << "-------------------" << std::endl;
  }
  if (ti.type == IF) {
    op = getIf(lexer);
    return op;
  }
  if (ti.type == WHILE) {
    op = getWhile(lexer);
    return op;
  }
  if (ti.type == DEF) {
    op = getFunctionDef(lexer);
    _functions[op->getName()] = std::unique_ptr<IOp>(op);
    //
    return new BasicOp(TokenInfo(NUMBER, "0", ti.line));
  }
  return new InvalidOp(SYNTAX, ti.line);
}


std::vector<IOp* > Parser::getExpressionsSequence(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  std::vector<IOp* > expr;
   //Operand o1 = first
  while (ti.type != ENDOFFILE && ti.type != END) {
    IOp* op = getNextExpression(lexer);
    expr.push_back(op);
    if (op->getLastError() != OK) {
      //clear vector
      //add only member - current error
      op->getErrorInfo();
      break;
    }
      //temporary
      //IOp* op = getCondition(lexer);
      //op->print();
      //std::cout << "-------------------" << std::endl;
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
  switch (ei.error_type()) {
    case ErrorType::UNKNOWN:
      std::cout << "unknown error occured.";
      break;
    case ErrorType::UNDEF_VARIABLE:
      std::cout << "undefined variable " << ei.error_info.location << ".";
      break;
    case ErrorType::UNDEF_FUNCTION:
      std::cout << "undefined function " << ei.error_info.location << ".";
      break;
    case ErrorType::SYNTAX:
      std::cout << "syntax error.";
      break;
    case ErrorType::NO_CONTEXT:
      std::cout << "context not found...";
      break;
    case ErrorType::FUNCTION_RETURN:
      std::cout << "function return statement occured.";
      break;
    case ErrorType::DIVISION_BY_ZERO:
      std::cout << "division by zero.";
      break;
    case ErrorType::ARGS_MISMATCH:
      std::cout << "arguments number mistmatch for "<< ei.error_info.location << ".";
      break;
  }
  std::cout << std::endl;
}