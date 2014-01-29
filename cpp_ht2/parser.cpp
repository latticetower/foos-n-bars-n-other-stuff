#include "parser.h"

IOp* Parser::getRead(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::READ) {
    lexer->getNextToken();
    TokenInfo var = lexer->getNextToken(); //TODO:add check
    IOp * op1 = new ReadOp(var);
    return op1;
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}

IOp* Parser::getPrint(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::PRINT) {
    lexer->getNextToken();
    IOp * op1 = getExpr(lexer);
    op1 = new PrintOp(op1);
    return op1;
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}

// =
IOp* Parser::getAssignment(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::VAR) {
    lexer->getNextToken();
    if (lexer->peekNextToken().first == TokenType::ASSIGN) {
      lexer->getNextToken();
      IOp * op1 = getExpr(lexer);
      op1 = new AssignOp(var, op1);
      return op1;
    }
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}


IOp* Parser::getExpr(Lexer* lexer) {
  //return getExprPrior2(l);
  return getExprPrior1(lexer);
}


// +, -
IOp* Parser::getExprPrior1(Lexer* lexer) {
  IOp* op1 = getExprPrior2(lexer);
  if (!op1->valid())
    return op1;
  TokenInfo current_operation = lexer->peekNextToken();
  while (current_operation.first  == TokenType::PLUS || current_operation.first == TokenType::MINUS) {
    lexer->getNextToken();
    IOp* op2 = getExprPrior2(lexer);
    if (current_operation.first == TokenType::PLUS) {
      op1 = new PlusOp(op1, op2);
    }
    if (current_operation.first == TokenType::MINUS) {
      op1 = new MinusOp(op1, op2);
    }
    current_operation = lexer->peekNextToken();
    if (current_operation.first == TokenType::ENDOFFILE)
      break;
  }
  return op1;
}

//  *, /
IOp* Parser::getExprPrior2(Lexer* lexer) {
  // 1. get first operand
  IOp* op1 = getExprPrior3(lexer);
  if (!op1->valid()) {
    return op1;
  }
  TokenInfo current_operation = lexer->peekNextToken();
  while (current_operation.first == TokenType::MULT || current_operation.first == TokenType::DIV) {
    lexer->getNextToken();
    IOp* op2 = getExprPrior3(lexer);
    if (current_operation.first == TokenType::MULT) {
      op1 = new MultOp(op1, op2);
    }
    if (current_operation.first == TokenType::DIV) {
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
  if (t.first == TokenType::LBRACKET) {
    lexer->getNextToken();
    result = getExprPrior1(lexer);
    lexer->getNextToken();//TODO: add check for rbracket
  }
  else {
    if (t.first!= TokenType::ENDOFFILE)
      result = new BasicOp(lexer->getNextToken());
    else {
      result = new InvalidOp(ErrorType::UNKNOWN);
    }
  }
  
  return result;
}

///conditions

IOp* Parser::getCondition(Lexer* lexer) {
  IOp * op1 = getExpr(lexer);
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::EQ ||
      var.first == TokenType::NEQ || 
      var.first == TokenType::GREATER ||
      var.first == TokenType::GEQ ||
      var.first == TokenType::LESS ||
      var.first == TokenType::LEQ) {

    lexer->getNextToken();
    IOp * op2 = getExpr(lexer);
    op1 = new ConditionOp(op1, op2, var);
    return op1;
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}


IOp* Parser::getIf(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::IF) {
    lexer->getNextToken();
    IOp* cond = getCondition(lexer);
    var = lexer->getNextToken();
    if (var.first != TokenType::COLON) {
      //report error
    }
    
    std::vector<IOp*> sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    return new IfOp(cond, sequence);
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}


IOp* Parser::getWhile(Lexer* lexer) {
  TokenInfo var = lexer->peekNextToken();
  if (var.first == TokenType::WHILE) {
    lexer->getNextToken();
    IOp* cond = getCondition(lexer);
    var = lexer->getNextToken();
    if (var.first != TokenType::COLON) {
      //report error
    }
    
    std::vector<IOp*> sequence = getExpressionsSequence(lexer);
    var = lexer->getNextToken();//END
    return new WhileOp(cond, sequence);
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}

IOp *Parser::getNextExpression(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  IOp* op;
  if (ti.first == TokenType::VAR) {
    op = getAssignment(lexer);
    return op;
    //op->print();
    //std::cout << "-------------------" << std::endl;   
  }
  if (ti.first == TokenType::PRINT) {
    op = getPrint(lexer);
    return op;
  //  op->print();
  //  std::cout << "-------------------" << std::endl;
  }
  if (ti.first == TokenType::READ) {
    op = getRead(lexer);
    return op;
    //op->print();
    //std::cout << "-------------------" << std::endl;
  }
  if (ti.first == TokenType::IF) {
    op = getIf(lexer);
    return op;
  }
  if (ti.first == TokenType::WHILE) {
    op = getWhile(lexer);
    return op;
  }
  return new InvalidOp(ErrorType::UNKNOWN);
}


std::vector<IOp*> Parser::getExpressionsSequence(Lexer* lexer) {
  TokenInfo ti = lexer->peekNextToken();
  std::vector<IOp*> expr;
   //Operand o1 = first
  while (ti.first != TokenType::ENDOFFILE && ti.first != TokenType::END) {
      IOp* op = getNextExpression(lexer);
      expr.push_back(op);
      //temporary
      //IOp* op = getCondition(lexer);
      //op->print();
      //std::cout << "-------------------" << std::endl;
      ti = lexer->peekNextToken();
    }
  return expr;
}

void Parser::ComputeAll(Context* context) {
  for (std::vector<IOp*>::iterator iter = _expressions.begin(); iter != _expressions.end(); ++iter) {
    (*iter)->Compute(context);
  }
}