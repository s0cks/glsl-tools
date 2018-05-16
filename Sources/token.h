#ifndef GLSLTOOLS_TOKEN_H
#define GLSLTOOLS_TOKEN_H

#include <string>
#include <sstream>
#include <cstring>

namespace GLSLTools{
#define FOR_EACH_KEYWORD(V) \
  V(kRETURN, "return") \
  V(kVEC2, "vec2") \
  V(kVEC3, "vec3") \
  V(kVEC4, "vec4")

#define FOR_EACH_SYMBOL(V) \
  V(kEQUALS, "=") \
  V(kLBRACE, "{") \
  V(kRBRACE, "}") \
  V(kSEMICOLON, ";") \
  V(kPLUS, "+") \
  V(kMINUS, "-") \
  V(kMUL, "*") \
  V(kDIVIDE, "/") \
  V(kLPAREN, "(") \
  V(kRPAREN, ")") \
  V(kCOMMA, ",")

#define FOR_EACH_LITERAL(V) \
  V(kLIT_STRING, "<literal string>") \
  V(kLIT_NUMBER, "<literal number>")

  enum TokenKind{
  #define DEFINE_TOKEN(Tk, Name) Tk,
    FOR_EACH_KEYWORD(DEFINE_TOKEN)
    FOR_EACH_SYMBOL(DEFINE_TOKEN)
    FOR_EACH_LITERAL(DEFINE_TOKEN)
  #undef DEFINE_TOKEN
    kIDENTIFIER,
    kINVALID,
    kEOF
  };

  struct SourcePosition{
    unsigned int row;
    unsigned int column;

    SourcePosition(unsigned int r, unsigned int c):
      row(r),
      column(c){}
  };

  class Token{
  private:
    std::string text_;
    TokenKind kind_;
    SourcePosition position_;
  public:
    Token(std::string text, TokenKind kind, SourcePosition* pos):
      text_(text),
      kind_(kind),
      position_(0, 0){
      if(pos != nullptr){
        std::memcpy(&position_, pos, sizeof(SourcePosition));
      }
    }
    Token(std::string text, TokenKind kind, unsigned int row, unsigned int column):
      text_(text),
      kind_(kind),
      position_(row, column){}
    Token(std::string text, TokenKind kind):
      text_(text),
      kind_(kind),
      position_(-1, -1){}

    std::string GetText() const{
      return text_;
    }

    unsigned int GetRow() const{
      return position_.row;
    }

    unsigned int GetColumn() const{
      return position_.column;
    }

    TokenKind GetKind() const{
      return kind_;
    }

    std::string GetKindDescription() const{
      #define DEFINE_SWITCH_CASE(Tk, Name) \
        case Tk: return #Name;

        switch(kind_){
          FOR_EACH_KEYWORD(DEFINE_SWITCH_CASE)
          FOR_EACH_SYMBOL(DEFINE_SWITCH_CASE)
          FOR_EACH_LITERAL(DEFINE_SWITCH_CASE)
          case kIDENTIFIER: return "<identifier>";
          case kEOF: return "<eof>";
          default: return "<unknown>";
        }
    }

    std::string GetPosition() const{
      std::stringstream stream;
      stream << "(" << position_.row << ", " << position_.column << ")";
      return stream.str();
    }

    std::string ToString() const{
      std::stringstream stream;
      stream << "{";
      stream << "\"text\"=" << text_;
      stream << ",\"kind\"=" << GetKindDescription();
      stream << ",\"position\"=" << GetPosition();
      stream << "}";
      return stream.str();
    }
  };
}

#endif //GLSLTOOLS_TOKEN_H
