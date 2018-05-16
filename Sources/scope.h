#ifndef GLSLTOOLS_SCOPE_H
#define GLSLTOOLS_SCOPE_H

#include <string>
#include "array.h"
#include "type.h"

namespace GLSLTools{
  class LocalScope;

  class LocalVariable{
  private:
    std::string name_;
    LocalScope* owner_;
    Type* type_;
    Value* value_;
  public:
    LocalVariable(std::string name, Type* type):
      name_(name),
      type_(type),
      value_(nullptr),
      owner_(nullptr){}

    std::string GetName() const{
      return name_;
    }

    LocalScope* GetOwner() const{
      return owner_;
    }

    Type* GetType() const{
      return type_;
    }

    Value* GetConstantValue() const{
      return value_;
    }

    bool IsConstant() const{
      return value_ != nullptr;
    }

    void SetOwner(LocalScope* owner){
      owner_ = owner;
    }

    void SetConstantValue(Value* val){
      value_ = val;
    }
  };

  class LocalScope{
  private:
    LocalScope* parent_;
    LocalScope* child_;
    LocalScope* sibling_;
    Array<LocalVariable*> locals_;
  public:
    LocalScope(LocalScope* parent = nullptr):
      parent_(parent),
      child_(nullptr),
      sibling_(nullptr),
      locals_(10){

      if(parent != nullptr){
        sibling_ = parent->child_;
        parent->child_ = this;
      }
    }

    LocalScope* GetParent() const{
      return parent_;
    }

    bool AddLocal(LocalVariable* local);
    bool HasLocal(std::string name);
    bool LocalLookup(std::string name, LocalVariable** result);
    bool Lookup(std::string name, LocalVariable** result);
  };
}

#endif //GLSLTOOLS_SCOPE_H
