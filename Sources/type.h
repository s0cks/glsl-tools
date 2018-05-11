#ifndef GLSLTOOLS_TYPE_H
#define GLSLTOOLS_TYPE_H

#include <string>

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
  };
}

#endif //GLSLTOOLS_TYPE_H
