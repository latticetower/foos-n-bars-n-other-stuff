#pragma once
#include "basic_types.h"
#include "lexer.h"
#include "context.h"
#include "base.h"
#include "io.h"
#include "operations.h"
#include "conditional.h"
#include "error_handling.h"



class Parser{
  Lexer* lexer;
  std::vector<IOp*> _expressions;
  TokenInfo lastOperand;
public:
  Parser(Lexer * l): lexer(l) {
    lastOperand = std::make_pair(TokenType::END, "");
  }
  
  std::vector<IOp*> getExpressionsSequence(Lexer* lexer);

  IOp* getNextExpression(Lexer* lexer);

  void Parse() {
   //Operand o1 = first
    _expressions = getExpressionsSequence(lexer);
    //for (std::vector<IOp*>::iterator iter = _expressions.begin(); iter != _expressions.end(); ++iter) {
    //  std::cout << "------------" << std::endl << "expr" << std::endl;
    //  (*iter)->print();
    //}
  }
  
  void ComputeAll(Context* c);


private:
  /*arithmetic operations*/
  IOp* getExpr(Lexer* lexer);
  IOp* getExprPrior1(Lexer* lexer);
  IOp* getExprPrior2(Lexer* lexer);
  IOp* getExprPrior3(Lexer* lexer);
  /**/
  IOp* getAssignment(Lexer* lexer);

  IOp* getRead(Lexer* lexer);
  IOp* getPrint(Lexer* lexer);
  //
  IOp* getCondition(Lexer*lexer);
  IOp* getIf(Lexer*lexer);
  IOp* getWhile(Lexer*lexer);

};