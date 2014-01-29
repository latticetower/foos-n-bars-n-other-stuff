#pragma once
#include <utility>
#include <string>

enum TokenType {
  VAR, NUMBER, ASSIGN, 
  RBRACKET, LBRACKET, 
  IF, WHILE, RETURN, DEF, COLON,
  PLUS, MINUS, MULT, DIV, 
  LESS, GREATER, LEQ, GEQ, EQ, NEQ, 
  READ, PRINT,
  END, 
  ENDOFFILE
};

typedef std::pair<TokenType, std::string> TokenInfo;

enum ErrorType {
  OK,
  SYNTAX,
  DIVISION_BY_ZERO,
  UNDEF_VARIABLE,
  UNDEF_FUNCTION,
  ARGS_MISMATCH, 
  UNKNOWN // in case if aliens stole my baby
};

typedef std::pair<ErrorType, int> VariableInfo;

