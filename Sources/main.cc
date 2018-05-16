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
  CodeUnit* code = parser.ParseUnit();
  code->GetFunction("main")->GetCode()->Visit(AstPrinter::SYS_OUT);
  return 0;
}
