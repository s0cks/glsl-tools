#ifndef GLSLTOOLS_TOKEN_H
#define GLSLTOOLS_TOKEN_H

#include <string>

namespace GLSLTools{
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
  V(kRPAREN, ")")

#define FOR_EACH_LITERAL(V) \
  V(kLIT_STRING, "<literal string>") \
  V(kLIT_NUMBER, "<literal number>")

  enum TokenKind{
  #define DEFINE_TOKEN(Tk, Name) Tk,
    FOR_EACH_SYMBOL(DEFINE_TOKEN)
    FOR_EACH_LITERAL(DEFINE_TOKEN)
  #undef DEFINE_TOKEN
    kIDENTIFIER,
    kEOF
  };

  struct SourcePosition{
    unsigned int row;
    unsigned int column;

    SourcePosition(unsigned int r, unsigned int c):
      row(r),
      column(c){}
  }

  class Token{
  private:
    std::string text_;
    TokenKind kind_;
    SourcePosition position_;
  public:
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
  };
}

#endif //GLSLTOOLS_TOKEN_H
