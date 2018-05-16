#include "ast_printer.h"
#include <iostream>

namespace GLSLTools{
  AstPrinter* AstPrinter::SYS_OUT = new AstPrinter(std::cout);
}
