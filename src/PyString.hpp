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

    // python的str只能和str加
    virtual PyObject* add(PyObject *lhs, PyObject *rhs);

    // python的str不支持减法
    // virtual PyObject* sub(PyObject *lhs, PyObject *rhs);

    // TODO 字符串不能乘以字符串 但是能乘以int 自动翻倍字符串...
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs);

    // python的str不支持除法
    // virtual PyObject* div(PyObject *lhs, PyObject *rhs);

    // python的str不支持mod
    // virtual PyObject* mod(PyObject *lhs, PyObject *rhs);

    // greater 只支持str和str比较
    virtual PyObject* greater(PyObject *lhs, PyObject *rhs);

    // less 只支持str和str比较
    virtual PyObject* less(PyObject *lhs, PyObject *rhs);

    // equal 支持str和任意类型比较
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs);

    // not equal 支持str和任意类型比较
    virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs);

    // ge 只支持str和str比较
    virtual PyObject* ge(PyObject *lhs, PyObject *rhs);

    // le 只支持str和str比较
    virtual PyObject* le(PyObject *lhs, PyObject *rhs);

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
