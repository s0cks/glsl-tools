#ifndef GLSLTOOLS_TYPE_H
#define GLSLTOOLS_TYPE_H

#include <string>
#include <cstring>
#include "array.h"

namespace GLSLTools{
  class Type;

  class Value{
  private:
    Type* type_;
    bool is_constant_;
    union{
      float float_value_;
      int int_value_;
    };
  public:
    Value(Type* type, bool is_constant):
      type_(type),
      is_constant_(is_constant){}
    ~Value(){}

    Type* GetType() const{
      return type_;
    }

    bool IsConstant() const{
      return is_constant_;
    }

    int AsInt() const{
      return int_value_;
    }

    float AsFloat() const{
      return float_value_;
    }

    std::string ToString();

    static Value* NewInstance(float floatValue, bool is_constant = false);
    static Value* NewInstance(int intValue, bool is_constant = false);
  };

  class Type{
  private:
    std::string name_;
    size_t size_;
    bool is_number_;

    Type(std::string name, size_t size, bool is_number):
      name_(name),
      size_(size),
      is_number_(is_number){}
  public:
    ~Type(){}

    size_t GetSize() const{
      return size_;
    }

    bool IsNumber() const{
      return is_number_;
    }

    bool IsCompatibile(const Type& other) const{
      return size_ == other.size_ &&
             is_number_ == other.is_number_;
    }

    std::string GetName() const{
      return name_;
    }

    static Type* FLOAT;
    static Type* INT;
    static Type* VEC2;
    static Type* VEC3;
    static Type* VEC4;
    static Type* VOID;
    static Type* ERROR;

    static Type* Get(std::string value){
      if(value == "void"){
        return VOID;
      } else if(value == "float"){
        return FLOAT;
      } else if(value == "vec2"){
        return VEC2;
      } else if(value == "vec3"){
        return VEC3;
      } else if(value == "vec4"){
        return VEC4;
      } else if(value == "int"){
        return INT;
      } else{
        return ERROR;
      }
    }
  };

  class SequenceNode;

  class Function{
  private:
    std::string name_;
    Type* result_type_;
    SequenceNode* code_;
  public:
    Function(std::string name, Type* result_type, SequenceNode* code);
    ~Function();

    Type* GetResultType() const{
      return result_type_;
    }

    std::string GetName() const{
      return name_;
    }

    SequenceNode* GetCode() const{
      return code_;
    }
  };

  class CodeUnit{
  private:
    Array<Function*> functions_;
  public:
    CodeUnit():
      functions_(10){}

    void AddFunction(Function* func){
      functions_.Add(func);
    }

    Function* GetFunction(std::string name){
      for(size_t i = 0; i < functions_.Length(); i++){
        if(functions_[i]->GetName() == name) return functions_[i];
      }
      return nullptr;
    }
  };
}

#endif //GLSLTOOLS_TYPE_H
