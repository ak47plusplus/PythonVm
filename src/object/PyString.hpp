#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

#include "PyObject.hpp"

/**
 *  @brief The Python String.
 *  @Created by GuoXiang 2019/10/29
 */
class PyString : public PyObject {
public:
  PyString(const char * str);
  PyString(const char * str, const int length);
  PyString(const PyString &rhs);
  PyString& operator=(const PyString& rhs);

  const char *value() { return this->m_value;}
  int length() {return this->m_length;}

private:
    char * m_value;
    int    m_length;
};

#endif
