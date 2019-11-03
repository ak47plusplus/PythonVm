#ifndef PY_STRING_HPP
#define PY_STRING_HPP

#include <mutex>      // for std::mutex and std::lock_guard

#include "Klass.hpp"
#include "PyObject.hpp"

/**
 *  @brief The Python String class and object.
 *  @Created by GuoXiang 2019/10/29
 */

class StringKlass : public Klass
{
public:
    static StringKlass *get_instance();

    virtual void print(PyObject *x);

    virtual PyObject* add(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* sub(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* mul(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* div(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* mod(PyObject *lhs, PyObject *rhs);


    // not supported: virtual PyObject* greater(PyObject *lhs, PyObject *rhs);
    virtual PyObject* less(PyObject *lhs, PyObject *rhs);
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* ge(PyObject *lhs, PyObject *rhs);
    // not supported: virtual PyObject* le(PyObject *lhs, PyObject *rhs);

private:
    StringKlass();
    static StringKlass *m_Instance;
    static std::mutex   m_Mutex;
};

class PyString : public PyObject {

public:
  PyString(const char * str);
  PyString(const char * str, const int length);
  PyString(const PyString& rhs);
  ~PyString();
  PyString& operator=(const PyString& rhs);
  const char *value() { return this->m_value;}
  int length()        {return this->m_length;}

private:
    char * m_value;
    int    m_length;
};

#endif
