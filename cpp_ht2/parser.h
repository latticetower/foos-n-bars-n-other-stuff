#pragma once
#include <memory>
#include <map>
#include "basic_types.h"
#include "lexer.h"
#include "context.h"
#include "base.h"
#include "io.h"
#include "operations.h"
#include "conditional.h"
#include "error_handling.h"
#include "functions.h"


class Parser{
  Lexer* lexer;
  std::vector<std::unique_ptr<IOp> > _expressions;
  std::map<std::string, std::unique_ptr<IOp> > _functions;
  // TokenInfo lastOperand;
public:
  Parser(Lexer * l): lexer(l) {
   // lastOperand = TokenInfo(END, "", 0); //TODO: check if unused
  }
  
  std::vector<IOp*> getExpressionsSequence(Lexer* lexer);

  IOp* getNextExpression(Lexer* lexer);

  void Parse() {
   //Operand o1 = first
    std::vector<IOp*> exp = getExpressionsSequence(lexer);
    _expressions.resize(exp.size());
    for (int i = 0; i < exp.size(); i++) {
      _expressions[i] = std::unique_ptr<IOp>(exp[i]);
    }
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
  IOp* getAssignment(Lexer* lexer, TokenInfo var);
  IOp* getFunctionCall(Lexer* lexer, TokenInfo var);
  IOp* getAssignmentOrFunctionCall(Lexer*lexer);

  IOp* getRead(Lexer* lexer);
  IOp* getPrint(Lexer* lexer);
  //
  IOp* getCondition(Lexer*lexer);
  IOp* getIf(Lexer*lexer);
  IOp* getWhile(Lexer*lexer);
  IOp* getFunctionDef(Lexer*lexer);
  IOp* getFunctionCall(Lexer*lexer);

};