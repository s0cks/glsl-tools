#include <iostream>
#include "ast.h"
#include "ast_printer.h"
#include "token.h"

using namespace GLSLTools;

int
main(int argc, char** argv){
  SequenceNode* seq = new SequenceNode();
  seq->Add(new ReturnNode(new LiteralNode(Value::NewInstance(10, true))));

  AstPrinter printer(std::cout);
  seq->Visit(&printer);

  Token* token = new Token("return",  kRETURN, 1, 1);
  std::cout << token->ToString() << std::endl;

  return 0;
}
