#ifndef PY_INTEGER_HPP
#define PY_INTEGER_HPP

#include "PyObject.hpp"

/**
 *  @brief The Python Integer.
 *  @Created by GuoXiang 2019/10/29
 */
class PyInteger : public PyObject {
public:
  PyInteger(int x) : m_value(x){}
  int value() {return this->m_value;}

  // tmp: Inherited from PyObject
  virtual void print();
  virtual PyObject* add(PyObject *rhs);
private:
  int m_value;
};

#endif
