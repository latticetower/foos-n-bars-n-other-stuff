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

//typedef std::pair<TokenType, std::string> TokenInfo;
struct TokenInfo {
  TokenType    type;
  std::string token;
  int line;
  TokenInfo(): type(NUMBER), token("0"), line(0) {}
  TokenInfo(TokenType _type, std::string _token, int _line): type(_type), token(_token), line(_line) { }
};

enum ErrorType {
  OK,
  SYNTAX,
  DIVISION_BY_ZERO,
  UNDEF_VARIABLE,
  UNDEF_FUNCTION,
  ARGS_MISMATCH, 
  UNKNOWN // in case if aliens stole my baby
};


struct ErrorInfo{
  ErrorType type;
  std::string location;
  int line;
  ErrorInfo(): type(OK), location(""), line(0) {}
  ErrorInfo(ErrorType et, int _line): type(et), location(""), line(_line) {}
  ErrorInfo(ErrorType et, int _line, std::string _location):  type(et), location(_location), line(_line) {}
  
  void setLocation(std::string const & _location) {
    location = std::string(_location);
  }
};

typedef std::pair<ErrorType, int> VariableInfo;