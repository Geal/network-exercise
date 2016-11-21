#ifndef OPTION_INC
#define OPTION_INC
#include <iostream>

template<class T>
class Option {
  public:
    virtual bool is_some() = 0;
    virtual T    get()     = 0;
    virtual      ~Option<T>() {}
};

template<class T>
class None: public Option<T> {
  public:
                 None<T>();
    virtual bool is_some();
    virtual T    get();
};

template<class T>
class Some: public Option<T> {
  private:
    T value;

  public:
                 Some<T>(T val);
    virtual bool is_some();
    virtual T    get();
};

template<class T>
None<T>::None() {
}

template<class T>
bool None<T>::is_some() {
  return false;
}

template<class T>
T None<T>::get() {
  throw;
}

template<class T>
Some<T>::Some(T val) {
  this->value = val;
}

template<class T>
bool Some<T>::is_some() {
  return true;
}

template<class T>
T Some<T>::get() {
  return value;
}

#endif
