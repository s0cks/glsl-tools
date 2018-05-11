#ifndef GLSLTOOLS_ARRAY_H
#define GLSLTOOLS_ARRAY_H

#include <cstdlib>

namespace GLSLTools{
  template<typename T>
  class Array{
  private:
    size_t length_;
    size_t capacity_;
    T* data_;

    void Resize(size_t nlen){
      if(nlen > capacity_){
        size_t ncap = RoundUpPowTwo(nlen);
        T* ndata = reinterpret_cast<T*>(realloc(data_, nlen * sizeof(T)));
        data_ = ndata;
        capacity_ = ncap;
      }
      length_ = nlen;
    }

    static size_t RoundUpPowTwo(size_t x){
      x = x -1;
      x = x | (x >> 1);
      x = x | (x >> 2);
      x = x | (x >> 4);
      x = x | (x >> 8);
      x = x | (x >> 16);
    #if defined(_M_X64) || defined(__x86_64__)
      x = x | (x >> 32);
    #endif
      return x + 1;
    }
  public:
    Array(size_t initCap):
      length_(0),
      capacity_(0),
      data_(nullptr){
        if(initCap > 0){
          capacity_ = RoundUpPowTwo(initCap);
          data_ = reinterpret_cast<T*>(malloc(capacity_ * sizeof(T)));
        }
    }
    ~Array(){
      free(data_);
    }

    size_t Length() const{
      return length_;
    }

    T& operator[](size_t index) const{
      return data_[index];
    }

    T& Last() const{
      return operator[](length_ - 1);
    }

    void Add(const T& value){
      Resize(Length() + 1);
      Last() = value;
    }

    void Clear(){
      length_ = 0;
    }

    bool IsEmpty() const{
      return Length() == 0;
    }
  };
}

#endif //GLSLTOOLS_ARRAY_H
