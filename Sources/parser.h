#ifndef GLSLTOOLS_PARSER_H
#define GLSLTOOLS_PARSER_H

#include "token.h"
#include "ast.h"
#include <string>
#include <vector>
#include <fstream>
#include <ctype.h>
#include <iostream>
#include <sstream>

namespace GLSLTools{
  class Parser{
  private:
    char* buffer_;
    size_t buffer_len_;
    size_t ptr_;
    SourcePosition position_;
    Token* peek_token_;

    inline char PeekChar(){
      if(ptr_ >= buffer_len_) return '\0';
      return buffer_[ptr_];
    }

    inline char NextChar(){
      if((ptr_ + 1) >= buffer_len_) return '\0';
      char c = buffer_[ptr_++];
      position_.column++;
      switch(c){
        case '\n':
          position_.column = 0;
          position_.row++;
        default: return c;
      }
    }

    inline Token* Expect(Token* next, TokenKind expected){
      std::cout << "Testing: " << next->GetText() << std::endl;
      if(next->GetKind() != expected){
        std::cerr << "Unexpected: " << next->ToString() << std::endl;
        std::cerr << "Expected: " << expected << std::endl;
        std::exit(1);
      }
      return next;
    }

    inline char NextRealChar(){
      char next;
      while(isspace(next = NextChar()));
      return next;
    }

    inline Token* PeekToken(){
      if(peek_token_ != nullptr){
        Token* result = peek_token_;
        peek_token_ = nullptr;
        return result;
      }
      return peek_token_ = NextToken();
    }

    inline BinaryOpNode::Kind GetBinaryExprKind(Token* token) const{
      switch(token->GetKind()){
        case kPLUS: return BinaryOpNode::kAdd;
        case kMINUS: return BinaryOpNode::kSubtract;
        default: return BinaryOpNode::kUnknown;
      }
    }

    inline bool HasDecimalPlace(std::string value) const{
      return value.find(".") != std::string::npos;
    }

    inline bool EndsWith(std::string value, const std::string& end) const{
      if(end.size() > value.size()) return false;
      return std::equal(value.begin() + value.size() - end.size(), value.end(), end.begin());
    }

    inline bool IsSymbolChar(char c) const{
      #define DECLARE_CHECK(Tk, Name) \
        if(std::string(1, c) == std::string(Name)){ \
          return true; \
        }
      FOR_EACH_SYMBOL(DECLARE_CHECK)
      #undef DECLARE_CHECK
      return false;
    }

    inline bool IsBinaryExpr(Token* token) const{
      switch(token->GetKind()){
        case kPLUS:
        case kMINUS: return true;
        default: return false;
      }
    }

    inline TokenKind GetKeyword(std::string text) const{
      #define DECLARE_CHECK(Tk, Name) \
        if(text == std::string(Name)) return Tk;
      FOR_EACH_KEYWORD(DECLARE_CHECK)
      #undef DECLARE_CHECK
      return kINVALID;
    }

    inline bool IsKeyword(std::string text) const{
      return GetKeyword(text) != kINVALID;
    }

    Token* NextToken();
    AstNode* ParseBinaryExpr();
    AstNode* ParseUnaryExpr();
    AstNode* ParseBlock();
  public:
    Parser(std::ifstream* infile):
      buffer_(nullptr),
      ptr_(0),
      buffer_len_(0),
      position_(0, 0),
      peek_token_(nullptr){

      if(*infile){
        std::filebuf* buff = infile->rdbuf();
        buffer_len_ = buff->pubseekoff(0, infile->end, infile->in);
        buff->pubseekpos(0, infile->in);
        buffer_ = reinterpret_cast<char*>(malloc(sizeof(char) * buffer_len_ + 1));
        buffer_[buffer_len_] = '\0';
        buff->sgetn(buffer_, buffer_len_);
        infile->close();

        std::cout << "Buffer Size: " << buffer_len_ << std::endl;
        std::cout << "Buffer: " << std::string(buffer_) << std::endl;
        std::cout << "End of buffer" << std::endl;
      } else{
        std::cerr << "Cannot initialize memory buffer for file" << std::endl;
      }
    }

    CodeUnit* ParseUnit();
  };
}

#endif //GLSLTOOLS_PARSER_H
