#ifndef GLSLTOOLS_ASTPRINTER_H
#define GLSLTOOLS_ASTPRINTER_H

#include "ast.h"
#include <iostream>
#include <string>

namespace GLSLTools{
  class AstPrinter : public AstNodeVisitor{
  private:
    std::ostream& stream_;
    int indent_ = 0;

    inline void Adjust(){
      for(int i = 0; i < indent_; i++) stream_ << " ";
    }
  public:
    static AstPrinter* SYS_OUT;

    AstPrinter(std::ostream& stream):
      stream_(stream){}
    ~AstPrinter(){}

    void VisitSequence(SequenceNode* node){
      Adjust();
      stream_ << "{" << std::endl;
      indent_++;
      node->VisitChildren(this);
      indent_--;
      stream_ << "}" << std::endl;
    }

    void VisitLiteral(LiteralNode* node){
      stream_ << node->GetValue()->ToString();
    }

    void VisitReturn(ReturnNode* node){
      Adjust();
      stream_ << "return ";
      node->VisitChildren(this);
      stream_ << ";" << std::endl;
    }

    void VisitBinaryOp(BinaryOpNode* node){
      node->GetLeft()->Visit(this);
      switch(node->GetKind()){
        case BinaryOpNode::kAdd: stream_ << " + "; break;
        case BinaryOpNode::kSubtract: stream_ << " - "; break;
        default: stream_ << " ? "; break;
      }
      node->GetRight()->Visit(this);
    }

    void VisitStoreLocal(StoreLocalNode* node){
      Adjust();
      stream_ << node->GetLocal()->GetName();
      stream_ << " := ";
      node->GetValue()->Visit(this);
      stream_ << ";" << std::endl;
    }

    void VisitLoadLocal(LoadLocalNode* node){
      stream_ << "$" << node->GetLocal()->GetName();
    }
  };
}

#endif //GLSLTOOLS_ASTPRINTER_H
