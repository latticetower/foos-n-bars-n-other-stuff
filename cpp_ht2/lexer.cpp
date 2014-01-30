#include "lexer.h"

Lexer::Lexer() {
  alphabet = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  other_useful_tokens = "!+-*/=><():";
  digits = "0123456789";
  std::string s2 = "+ - * / = == >= <= != > < ( ) :";
  std::istringstream iss(s2);
  rule_table.insert(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>());

  current_string = 0;
  current_token = 0;
}

void Lexer::printTokensToConsole(std::ostream& os) {
  for (std::vector<std::vector<TokenInfo > >::iterator i = token_strings_list.begin(); i!= token_strings_list.end(); ++ i) {
    for (std::vector<TokenInfo >::iterator i2 = i->begin(); i2 != i->end(); ++ i2) {
      os << i2->token << ", ";
    }
    os << std::endl;
  }
}

bool Lexer::isLastTokenInString() {
  if (current_string < (int)token_strings_list.size()) {
    if (current_token + 1 < (int)token_strings_list[current_string].size()) {
      return false;
    }
  }
  return true;
}


TokenInfo Lexer::peekNextToken() {
  while (current_string < (int)token_strings_list.size() && current_token >= (int)token_strings_list[current_string].size()) {
    current_string += 1;
    current_token = 0;
  }
    
  if (current_string < (int)token_strings_list.size()) {
    std::vector<TokenInfo > str = token_strings_list[current_string];
    return str[current_token];
  }
  return TokenInfo(ENDOFFILE, "", 0);
}

TokenInfo Lexer::getNextToken() {
  TokenInfo t = peekNextToken();
  current_token += 1;
  return t;
}

void Lexer::processString(std::string const& unprocessed_string) {
  char letter, next_letter;
  std::string buffer = "";
  std::vector<TokenInfo> tokens;
  int line_no = token_strings_list.size() + 1;

  for (int i = 0; i < (int)unprocessed_string.length(); ++i) {
    letter = unprocessed_string[i];
    if (letter == '#')
      break;
      
    next_letter = unprocessed_string[i + 1];

    if (alphabet.find(letter) != std::string::npos) {
      if (rule_table.find(buffer) != rule_table.end() ) {
        appendToken(tokens, buffer, line_no);
        buffer = "";
      }
      buffer.push_back(letter);
      if (alphabet.find(next_letter) == std::string::npos) {
        appendToken(tokens, buffer, line_no);
        buffer = "";
      }
    }

    if (other_useful_tokens.find(letter) != std::string::npos) {
      buffer.push_back(letter);

      std::string temp(buffer);
      temp.push_back(next_letter);
      if (rule_table.find(buffer) != rule_table.end() && rule_table.find(temp) == rule_table.end()) {
        appendToken(tokens, buffer, line_no);
        buffer = "";
      }
    }
    //
  }
  token_strings_list.push_back(tokens);
}


bool Lexer::loadFromFile(const char* filename) {
  token_strings_list.clear();

  std::ifstream inputStream(filename );
  if (inputStream.fail())
    return false;
  
  std::string buffer = "";
 
  while (!inputStream.eof()) {
    std::getline(inputStream, buffer);

    processString(buffer);
     
  }
  inputStream.close();
  return true;
}

void Lexer::appendToken(std::vector<TokenInfo > & tokens_list, const std::string buffer, int line_no) {
  if (buffer.length() > 0) {
    TokenType t;
    if (alphabet.find(buffer[0]) != alphabet.npos)
      t = VAR;
    if (digits.find(buffer[0]) != digits.npos)
      t = NUMBER;
    if (buffer == "if")
      t = IF; 
    if (buffer == "while")
      t = WHILE; 
    if (buffer == "end")
      t = END; 
    if (buffer =="return")
      t = RETURN;
    if (buffer == "def")
      t = DEF; 
    if (buffer == "read")
      t = READ; 
    if (buffer == "print")
      t = PRINT; 
    if (buffer == ":")
      t = COLON;
    if (buffer == ">")
      t = GREATER; 
    if (buffer == "<")
      t = LESS;
    if (buffer == "=")
      t = ASSIGN; 
    if (buffer == ">=")
      t = GEQ; 
    if (buffer == "<=")
      t = LEQ;
    if (buffer == "==")
      t = EQ;
    if (buffer == "!=")
      t = NEQ;

    if (buffer == "+")
      t = PLUS; 
    if (buffer == "-")
      t = MINUS; 
    if (buffer == "*")
      t = MULT; 
    if (buffer == "/")
      t = DIV; 

    if (buffer == "(")
      t = LBRACKET; 
    if (buffer == ")")
      t = RBRACKET; 
      
    tokens_list.push_back(TokenInfo(t, std::string(buffer), line_no));
  }
}