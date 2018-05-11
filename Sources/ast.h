#ifndef GLSLTOOLS_AST_H
#define GLSLTOOLS_AST_H

#include "array.h"
#include "type.h"

namespace GLSLTools{
  #define FOR_EACH_NODE(V) \
    V(Return) \
    V(Literal) \
    V(Sequence)

    #define DECLARE_COMMON_NODE_FUNCTIONS(BaseName) \
      virtual const char* Name(){ return #BaseName; } \
      virtual void Visit(AstNodeVisitor* vis); \
      virtual BaseName##Node* As##BaseName(){ return this; }

    #define FORWARD_DECLARE(BaseName) class BaseName##Node;
      FOR_EACH_NODE(FORWARD_DECLARE)
    #undef FORWARD_DECLARE

    class AstNodeVisitor{
    public:
      AstNodeVisitor(){}
      virtual ~AstNodeVisitor(){}

      #define DECLARE_VISIT_FUNCTION(BaseName) \
        virtual void Visit##BaseName(BaseName##Node* node){}

        FOR_EACH_NODE(DECLARE_VISIT_FUNCTION)
      #undef DECLARE_VISIT_FUNCTION
    };

    class AstNode{
    public:
      AstNode(){}
      virtual ~AstNode(){}

      #define DEFINE_TYPE_CHECK(BaseName) \
        bool Is##BaseName(){ return As##BaseName() != nullptr; } \
        virtual BaseName##Node* As##BaseName(){ return nullptr; }

        FOR_EACH_NODE(DEFINE_TYPE_CHECK)
      #undef DEFINE_TYPE_CHECK

        virtual const char* Name() = 0;
        virtual void Visit(AstNodeVisitor* vis) = 0;
        virtual void VisitChildren(AstNodeVisitor* vis) = 0;

        virtual bool IsConstantExpr() const{
          return false;
        }
    };

    class SequenceNode : public AstNode{
    private:
      Array<AstNode*> children_;
    public:
      SequenceNode():
        children_(10){}

      void Add(AstNode* node){
        children_.Add(node);
      }

      void VisitChildren(AstNodeVisitor* vis){
        for(int i = 0; i < children_.Length(); i++){
          children_[i]->Visit(vis);
        }
      }

      AstNode* GetChildAt(size_t idx) const{
        return children_[idx];
      }

      size_t GetChildrenSize() const{
        return children_.Length();
      }

      DECLARE_COMMON_NODE_FUNCTIONS(Sequence);
    };

    class LiteralNode : public AstNode{
    private:
      Value* value_;
    public:
      LiteralNode(Value* value):
        value_(value){}
      ~LiteralNode(){}

      Value* GetValue() const{
        return value_;
      }

      void VisitChildren(AstNodeVisitor* vis){}

      DECLARE_COMMON_NODE_FUNCTIONS(Literal);
    };

    class ReturnNode : public AstNode{
    private:
      AstNode* value_;
    public:
      ReturnNode(AstNode* value):
        value_(value){}
      ~ReturnNode(){}

      AstNode* GetValue() const{
        return value_;
      }

      void VisitChildren(AstNodeVisitor* vis){
        GetValue()->Visit(vis);
      }

      DECLARE_COMMON_NODE_FUNCTIONS(Return);
    };
}

#endif //GLSLTOOLS_AST_H
