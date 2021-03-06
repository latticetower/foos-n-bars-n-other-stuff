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
  if (argc < 2) {
    std::cout << "Please, provide file name as a parameter" << std::endl;
    return 0;
  }
  if (!lexer.loadFromFile(argv[1])) {
    std::cout << "Please, provide _valid_ file name as a parameter" << std::endl;
    return 0;
  }
  
  Parser p(&lexer);
  if (!p.Parse())
    return 0;
  // p.PrintAll(&std::cout);
  p.ComputeAll(&globalContext);

  return 0;
}