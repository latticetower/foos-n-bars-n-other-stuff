#include <string>
#include <fstream>
#include <iostream>

#include "lexer.h"
#include "parser.h"

Lexer lexer;

int main(int argc, char**argv) {
  std::string s;
  lexer.loadFromFile(argv[1]);
  lexer.printTokensToConsole(std::cout);
  Parser p(&lexer);
  p.Parse();
  return 0;
}