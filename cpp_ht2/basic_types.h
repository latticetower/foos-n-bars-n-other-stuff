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
  FUNCTION_RETURN, //for handling return statements in functions
  NO_CONTEXT, // for handling cases when in Compute method context parameter is null
  UNKNOWN // in case if aliens stole my baby
};


struct ErrorInfo{
  ErrorType type;
  int line;

  ErrorInfo(): type(UNKNOWN), line(0) {}
  ErrorInfo(int _line): type(UNKNOWN), line(_line) {}
  ErrorInfo(ErrorType et, int _line): type(et), line(_line) {}
};

struct ResultInfo{
  ErrorInfo error_info;
  std::string object_name;
  int result;
  ResultInfo(int line): error_info(UNKNOWN, line), result(0) {}
  ResultInfo(int _result, int _line): error_info(OK, _line), result(_result) { }
  ResultInfo(int _result, int _line, ErrorType info): error_info(info, _line), result(_result) { }
  ResultInfo(int _result, int _line, ErrorType info, std::string const& name): error_info(info, _line), object_name(name), 
                                result(_result) { }

  ErrorType error_type() {
    return error_info.type;
  }
  ResultInfo operator == (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result == res2.result, error_info.line);
  }
  ResultInfo operator < (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result < res2.result, error_info.line);
  }

  ResultInfo operator > (ResultInfo res2) {
    return res2 < *this;
  }

  ResultInfo operator != (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result != res2.result, error_info.line);
  }

  ResultInfo operator >= (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result >= res2.result, error_info.line);
  }
  ResultInfo operator <= (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result <= res2.result, error_info.line);
  }

  ResultInfo operator + (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result + res2.result, error_info.line);
  }
  ResultInfo operator - (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result - res2.result, error_info.line);
  }
  ResultInfo operator * (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    return ResultInfo(result * res2.result, error_info.line);
  }

  ResultInfo operator / (ResultInfo res2) {
    if (error_info.type != OK) {
      return *this;
    }
    if (res2.error_type() != OK) {
      return res2;
    }
    if (res2.result == 0) {
      return ResultInfo(0, res2.error_info.line, DIVISION_BY_ZERO);
    }
    return ResultInfo(result / res2.result, error_info.line);
  }
};
//typedef std::pair<ErrorInfo, int> ResultInfo;