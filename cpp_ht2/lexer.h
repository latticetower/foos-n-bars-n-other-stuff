#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>

#include "basic_types.h"



class Lexer{
  std::set<std::string> rule_table; 
  std::vector<std::vector<TokenInfo > > token_strings_list;
  std::string alphabet, other_useful_tokens, digits;
  int current_string, current_token;

  void appendToken(std::vector<TokenInfo >& tokens_list, const std::string buffer, int line_no);

public:  
  Lexer();

  //method for debug purposes
  void printTokensToConsole(std::ostream& os);

  //method checks if next token is last in string
  bool isLastTokenInString();
  
  //method returns next token from file with its TokenType value. doesn't move current position
  TokenInfo peekNextToken();

  //method returns next parsed token and moves current position
  TokenInfo getNextToken();

  //method gets string and split it to tokens, with the help of rules given in class constructor
  void processString(const std::string& unprocessed_string);

  //method opens given file and loads all tokens from it, string by string
  void loadFromFile(const char* filename);
};