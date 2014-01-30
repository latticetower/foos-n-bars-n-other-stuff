#include <string>
#include <fstream>
#include <iostream>

#include "basic_types.h"
#include "context.h"
#include "lexer.h"
#include "parser.h"



Lexer lexer;
Context globalContext(NULL);

int main(int argc, char**argv) {
  std::string s;
  lexer.loadFromFile(argv[1]);
  //lexer.printTokensToConsole(std::cout);
  Parser p(&lexer);
  if (!p.Parse())
    return 0;
  p.ComputeAll(&globalContext);

  return 0;
}