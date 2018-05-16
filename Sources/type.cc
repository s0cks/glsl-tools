#include "type.h"
#include "ast.h"
#include <sstream>

namespace GLSLTools{
  Type* Type::FLOAT = new Type("float", 1, false);
  Type* Type::INT = new Type("int", 1, true);
  Type* Type::VEC2 = new Type("vec2", 2, false);
  Type* Type::VEC3 = new Type("vec3", 3, false);
  Type* Type::VEC4 = new Type("vec4", 4, false);
  Type* Type::VOID = new Type("void", 0, true);
  Type* Type::ERROR = new Type("__ERROR__", 0, true);

  Value* Value::NewInstance(float value, bool is_constant){
    Value* val = new Value(Type::FLOAT, is_constant);
    val->float_value_ = value;
    return val;
  }

  Value* Value::NewInstance(int value, bool is_constant){
    Value* val = new Value(Type::INT, is_constant);
    val->int_value_ = value;
    return val;
  }

  Value* Value::NewVector(size_t size){
    Value* val;
    switch(size){
      case 2: val = new Value(Type::VEC2, false); break;
      case 3: val = new Value(Type::VEC3, false); break;
      case 4: val = new Value(Type::VEC4, false); break;
      default: return nullptr;
    }
    val->vec_value_.values = reinterpret_cast<Value**>(malloc(sizeof(Value*) * size));
    val->vec_value_.values_len = size;
    return val;
  }

  bool Value::IsScalar() const{
    return GetType()->IsCompatibile(*Type::VEC2) ||
           GetType()->IsCompatibile(*Type::VEC3) ||
           GetType()->IsCompatibile(*Type::VEC4);
  }

  size_t Value::GetScalarSize() const{
    return IsScalar() ?
           vec_value_.values_len :
           0;
  }

  std::string Value::ToString(){
    std::stringstream stream;
    if(IsConstant()){
      if(GetType()->IsCompatibile((*Type::FLOAT))){
        stream << AsFloat();
      } else if(GetType()->IsCompatibile((*Type::INT))){
        stream << AsInt();
      }
    } else if(IsScalar()){
      stream << "vec" << GetScalarSize() << "(";
      for(size_t i = 0; i < GetScalarSize(); i++){
        if(GetAt(i) == nullptr){
          stream << "null";
        } else{
          stream << GetAt(i)->ToString();
        }
        if(i < (GetScalarSize() - 1)){
          stream << ", ";
        }
      }
      stream << ")";
    } else{
      stream << "Type[" << GetType()->GetName() << "]";
    }
    return stream.str();
  }

  Function::Function(std::string name, Type* result_type, SequenceNode* code = new SequenceNode()):
    name_(name),
    result_type_(result_type),
    code_(code){}

  Function::~Function(){
    delete code_;
  }
}
