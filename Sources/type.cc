#include "type.h"
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

  std::string Value::ToString(){
    std::stringstream stream;
    if(IsConstant()){
      if(GetType()->IsCompatibile((*Type::FLOAT))){
        stream << AsFloat();
      } else if(GetType()->IsCompatibile((*Type::INT))){
        stream << AsInt();
      }
    } else{
      stream << "Type[" << GetType()->GetName() << "]";
    }
    return stream.str();
  }
}
