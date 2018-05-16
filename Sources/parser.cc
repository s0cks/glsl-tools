#include "parser.h"
#include <sstream>

namespace GLSLTools{
  Token* Parser::NextToken(){
    if(peek_token_ != nullptr){
      Token* result = peek_token_;
      peek_token_ = nullptr;
      return result;
    }

    char next = NextRealChar();
    switch(next){
      case '\0': return new Token("\0", kEOF, &position_);
      case '=': return new Token("=", kEQUALS, &position_);
      case ',': return new Token(",", kCOMMA, &position_);
      case '+': return new Token("+", kPLUS, &position_);
      case '{': return new Token("{", kLBRACE, &position_);
      case '}': return new Token("}", kRBRACE, &position_);
      case '(': return new Token("(", kLPAREN, &position_);
      case ')': return new Token(")", kRPAREN, &position_);
      case '"':{
        std::stringstream stream;
        while((next = NextChar()) != '"') stream << next;
        return new Token(stream.str(), kLIT_STRING, &position_);
      }
      default: break;
    }

    if(isdigit(next) || next == '.'){
      std::stringstream stream;
      stream << next;
      while(isdigit(next = PeekChar()) || next == '.' || next == 'f' || next == 'F') stream << NextChar();
      return new Token(stream.str(), kLIT_NUMBER, &position_);
    } else{
      std::stringstream stream;
      stream << next;

      while(!isspace(next = PeekChar()) && !IsSymbolChar(next)){
        stream << NextChar();
        if(IsKeyword(stream.str())){
          std::string val = stream.str();
          return new Token(val, GetKeyword(val), &position_);
        }
      }
      return new Token(stream.str(), kIDENTIFIER, &position_);
    }
  }

  AstNode* Parser::ParseBinaryExpr(){
    AstNode* expr = ParseUnaryExpr();
    while(IsBinaryExpr(PeekToken())){
      expr = new BinaryOpNode(GetBinaryExprKind(NextToken()), expr, ParseBinaryExpr());
    }
    return expr;
  }

  AstNode* Parser::ParseUnaryExpr(){
    Token* next;
    switch((next = NextToken())->GetKind()){
      case kLIT_NUMBER:{
        if(HasDecimalPlace(next->GetText())){
          if(EndsWith(next->GetText(), "f") || EndsWith(next->GetText(), "F")){
            float val = atof(next->GetText().substr(next->GetText().size() - 1).c_str());
            return new LiteralNode(Value::NewInstance(val, true));
          } else{
            float val = atof(next->GetText().c_str());
            return new LiteralNode(Value::NewInstance(val, true));
          }
        } else{
          int val = atoi(next->GetText().c_str());
          return new LiteralNode(Value::NewInstance(val, true));
        }
      }
    }
  }

  AstNode* Parser::ParseBlock(){
    SequenceNode* code = new SequenceNode();

    Token* next;
    while((next = NextToken())->GetKind() != kRBRACE){
      switch(next->GetKind()){
        case kRETURN: code->Add(new ReturnNode(ParseBinaryExpr())); break;
        default: std::cerr << "Invalid Token: " << next->ToString() << std::endl;
      }
    }

    return code;
  }

  CodeUnit* Parser::ParseUnit(){
    CodeUnit* unit = new CodeUnit();

    Token* next;
    while((next = NextToken())->GetKind() != kEOF){
      switch(next->GetKind()){
        case kIDENTIFIER:{
          std::string type = next->GetText();
          std::string name = Expect(next = NextToken(), kIDENTIFIER)->GetText();

          std::cout << "Type: " << type << std::endl;
          std::cout << "Name: " << name << std::endl;

          Expect(next = NextToken(), kLPAREN);
          Expect(next = NextToken(), kRPAREN);
          Expect(next = NextToken(), kLBRACE);
          std::cout << "Parsing block" << std::endl;
          unit->AddFunction(new Function(name, Type::Get(type), static_cast<SequenceNode*>(ParseBlock())));
          break;
        }
      }
    }

    return unit;
  }
}
