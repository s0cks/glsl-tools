#include "scope.h"

namespace GLSLTools{
  bool LocalScope::Lookup(std::string name, LocalVariable** result){
    LocalScope* curr = this;
    while(curr != nullptr){
      if(curr->LocalLookup(name, result)){
        return true;
      }
      curr = curr->GetParent();
    }
    *result = nullptr;
    return false;
  }

  bool LocalScope::LocalLookup(std::string name, LocalVariable** result){
    for(int i = 0; i < locals_.Length(); i++){
      if(locals_[i]->GetName() == name){
        *result = locals_[i];
        return true;
      }
    }
    *result = nullptr;
    return false;
  }

  bool LocalScope::HasLocal(std::string name){
    LocalVariable* result = nullptr;
    Lookup(name, &result);
    return result != nullptr;
  }

  bool LocalScope::AddLocal(LocalVariable* local){
    if(HasLocal(local->GetName())) return false;
    locals_.Add(local);
    if(local->GetOwner() == nullptr) local->SetOwner(this);
    return true;
  }
}
