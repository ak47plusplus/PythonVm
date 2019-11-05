#ifndef PY_INTEGER_HPP
#define PY_INTEGER_HPP

#include "Klass.hpp"
#include "PyObject.hpp"

#include <mutex>


/**
 * <li> PyInteger的所有逻辑运算和算术运算符都支持和PyDouble进行且提升运算级 </li>
 * <li> PyInteger的+运算不能和字符串进行 </li>
 * <li> PyInteger的*运算可以和字符串进行表示翻倍 "aa"*3 = "aaaaaa"</li>
 */
class IntegerKlass : public Klass {
public:
    static IntegerKlass *get_instance();

    virtual void print(PyObject *x);

    virtual PyObject* add(PyObject *lhs, PyObject *rhs);
    virtual PyObject* sub(PyObject *lhs, PyObject *rhs);
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs);
    // python的除法比较坑爹 两个整数无论是否除尽都是浮点数
    virtual PyObject* div(PyObject *lhs, PyObject *rhs);
    virtual PyObject* mod(PyObject *lhs, PyObject *rhs);

    virtual PyObject* greater(PyObject *lhs, PyObject *rhs);
    virtual PyObject* less(PyObject *lhs, PyObject *rhs);
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs);
    virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs);
    virtual PyObject* ge(PyObject *lhs, PyObject *rhs);
    virtual PyObject* le(PyObject *lhs, PyObject *rhs);

private:
    IntegerKlass();
    static IntegerKlass *m_Instance;
    static std::mutex    m_Mutex;
};

/**
 *  @brief The Python Integer.
 *  @Created by GuoXiang 2019/10/29
 */
class PyInteger : public PyObject {
public:
  PyInteger(int x);
  int value() {return this->m_Value;}

private:
  int m_Value;
};

#endif
