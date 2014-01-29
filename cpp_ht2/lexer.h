#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>

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

class Lexer{

  std::set<std::string> rule_table; 
  std::vector<std::vector<TokenInfo > > token_strings_list;
  std::string alphabet, other_useful_tokens, digits;

  int current_string, current_token;

  void appendToken(std::vector<TokenInfo > & tokens_list, const std::string buffer) {
    if (buffer.length() > 0) {
      TokenType t;
      if (alphabet.find(buffer[0]) != alphabet.npos)
        t = TokenType::VAR;
      if (digits.find(buffer[0]) != digits.npos)
        t = TokenType::NUMBER;
      if (buffer == "if")
        t = TokenType::IF; 
      if (buffer == "while")
        t = TokenType::WHILE; 
      if (buffer == "end")
        t = TokenType::END; 
      if (buffer =="return")
        t = TokenType::RETURN;
      if (buffer == "def")
        t = TokenType::DEF; 
      if (buffer == "read")
        t = TokenType::READ; 
      if (buffer == "print")
        t = TokenType::PRINT; 
      if (buffer == ":")
        t = TokenType::COLON;
      if (buffer == ">")
        t = TokenType::GREATER; 
      if (buffer == "<")
        t = TokenType::LESS;
      if (buffer == "=")
        t = TokenType::ASSIGN; 
      if (buffer == ">=")
        t = TokenType::GEQ; 
      if (buffer == "<=")
        t = TokenType::LEQ;
      if (buffer == "==")
        t = TokenType::EQ;
      if (buffer == "!=")
        t = TokenType::NEQ;

      if (buffer == "+")
        t = TokenType::PLUS; 
      if (buffer == "-")
        t = TokenType::MINUS; 
      if (buffer == "*")
        t = TokenType::MULT; 
      if (buffer == "/")
        t = TokenType::DIV; 

      if (buffer == "(")
        t = TokenType::LBRACKET; 
      if (buffer == ")")
        t = TokenType::RBRACKET; 
      
      tokens_list.push_back(std::make_pair(t, std::string(buffer)));
    }
  }

public:  
  Lexer() {
    alphabet = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    other_useful_tokens = "+-*/=><():";
    digits = "0123456789";
    std::string s2 = "+ - * / = == >= <= != > < ( ) :";
    std::istringstream iss(s2);
    rule_table.insert(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>());

    current_string = 0;
    current_token = 0;
  }

  //method for debug purposes
  void printTokensToConsole(std::ostream& os) {
    for (std::vector<std::vector<TokenInfo > >::iterator i = token_strings_list.begin(); i!= token_strings_list.end(); ++ i) {
      for (std::vector<TokenInfo >::iterator i2 = i->begin(); i2 != i->end(); ++ i2) {
        os << i2->second << ", ";
      }
      os << std::endl;
    }
  }

  //method checks if next token is last in string
  bool isLastTokenInString() {
    if (current_string < token_strings_list.size()) {
      if (this->current_token + 1 < token_strings_list[current_string].size()) {
        return false;
      }
    }
    return true;
  }
  
  //method returns next token from file with its TokenType value. doesn't move current position
  TokenInfo peekNextToken() {
    while (current_string < token_strings_list.size() && current_token >= token_strings_list[current_string].size()) {
      current_string += 1;
      current_token = 0;
    }
    
    if (current_string < token_strings_list.size()) {
      std::vector<TokenInfo > str = token_strings_list[current_string];
      return str[current_token];
    }
    return std::make_pair(TokenType::ENDOFFILE, "");
  }

  //method returns next parsed token and moves current position
  TokenInfo getNextToken() {
    TokenInfo t = peekNextToken();
    current_token += 1;
    return t;
  }

  //method gets string and split it to tokens, with the help of rules given in class constructor
  void processString(const std::string& unprocessed_string) {
    char letter, next_letter;
    std::string buffer = "";
    std::vector<TokenInfo > tokens;

    for (int i = 0; i < unprocessed_string.length(); i++) {
      letter = unprocessed_string[i];
      if (letter == '#')
        break;
      
      next_letter = unprocessed_string[i + 1];

      if (alphabet.find(letter) != std::string::npos) {
        if (rule_table.find(buffer) != rule_table.end() ) {
          appendToken(tokens, buffer);
          buffer = "";
        }
        buffer.push_back(letter);
        if (alphabet.find(next_letter) == std::string::npos) {
          appendToken(tokens, buffer);
          buffer = "";
        }
      }

      if (other_useful_tokens.find(letter) != std::string::npos) {
        buffer.push_back(letter);

        std::string temp(buffer);
        temp.push_back(next_letter);
        if (rule_table.find(buffer) != rule_table.end() && rule_table.find(temp) == rule_table.end()) {
          appendToken(tokens, buffer);
          buffer = "";
        }
      }

      //
    }

    token_strings_list.push_back(tokens);

  }

  //method opens given file and loads all tokens from it, string by string
  void loadFromFile(const char* filename) {
    token_strings_list.clear();

    std::ifstream inputStream(filename );
    std::string buffer = "";
    char letter, next_letter;

    while (!inputStream.eof()) {
      std::getline(inputStream, buffer);

      processString(buffer);
     
   }
    inputStream.close();
  }
};