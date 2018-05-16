#include "parser.h"
#include "ast_printer.h"
#include <fstream>
#include <iostream>

using namespace GLSLTools;

int
main(int argc, char** argv){
  std::ifstream stream;
  stream.open(argv[1], std::ifstream::binary);

  std::cout << "Opening file: " << argv[1] << std::endl;

  Parser parser(&stream);
  AstNode* code = parser.ParseUnit();
  AstPrinter printer(std::cout);
  code->Visit(&printer);
  return 0;
}
