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
      case ';': return new Token(";", kSEMICOLON, &position_);
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

  Value* Parser::ParseVector(int vec_type){
    Token* next;
    Expect(next = NextToken(), kLPAREN);
    switch(vec_type){
      case 2:
      case 3:
      case 4:{
        Value* res = Value::NewVector(vec_type);
        int ptr = 0;
        while((next = PeekToken())->GetKind() != kRPAREN){
          res->SetAt(ptr++, ParseLiteral());
          switch((next = PeekToken())->GetKind()){
            case kCOMMA: break;
            case kRPAREN: return res;
            default: return res;
          }
        }
      }
      default:{
        Array<Value*> values(10);
        while((next = PeekToken())->GetKind() != kRPAREN){
          values.Add(ParseLiteral());
        }

        Value* res = Value::NewVector(values.Length() - 1);
        for(int i = 0; i < values.Length(); i++){
          res->SetAt(i, values[i]);
        }

        return res;
      }
    }
  }

  Value* Parser::ParseLiteral(){
    Token* next;
    switch((next = NextToken())->GetKind()){
      case kLIT_NUMBER:{
        std::cout << "Parsing literal nummber: " << next->GetText() << std::endl;
        std::string text = next->GetText();
        if(HasDecimalPlace(next->GetText())){
          float val;
          if(EndsWith(text, "f") || EndsWith(text, "F")){
            val = atof(text.substr(text.size() - 1).c_str());
          } else{
            val = atof(text.c_str());
          }
          return Value::NewInstance(val, true);
        } else{
          int val = atoi(text.c_str());
          return Value::NewInstance(val, true);
        }
      }
      case kVEC2: return ParseVector(2);
      case kVEC3: return ParseVector(3);
      case kVEC4: return ParseVector(4);
      default: {
        std::cerr << "Unexpected token: " << next->ToString() << std::endl;
        return nullptr;
      }
    }
  }

  AstNode* Parser::ParseUnaryExpr(){
    AstNode* result;

    Token* next;
    switch((next = PeekToken())->GetKind()){
      default: break;
    }

    result = new LiteralNode(ParseLiteral());

    switch((next = PeekToken())->GetKind()){
      default: return result;
    }
  }

  AstNode* Parser::ParseBlock(){
    SequenceNode* code = new SequenceNode(scope_);
    scope_ = code->GetScope();

    Token* next;
    while((next = NextToken())->GetKind() != kRBRACE){
      switch(next->GetKind()){
        case kRETURN:{
          code->Add(new ReturnNode(ParseBinaryExpr()));
          Expect(next = NextToken(), kSEMICOLON);
          break;
        }
        case kIDENTIFIER:{
          std::string name = next->GetText();
          Expect(next = NextToken(), kEQUALS);

          LocalVariable* local;
          if(!scope_->Lookup(name, &local)){
            std::cerr << "Undefined local: " << name << std::endl;
            std::exit(1);
            return nullptr;
          }
          code->Add(new StoreLocalNode(local, ParseBinaryExpr()));
          Expect(next = NextToken(), kSEMICOLON);
          break;
        }
        default: std::cerr << "Invalid Token: " << next->ToString() << std::endl;
      }
    }

    scope_ = scope_->GetParent();
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
