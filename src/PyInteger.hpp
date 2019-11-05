#ifndef PY_INTEGER_HPP
#define PY_INTEGER_HPP

#include "Klass.hpp"
#include "PyObject.hpp"

#include <mutex>


/**
 *  @brief The Python Integer Class and Integer Object.
 *  @Created by GuoXiang 2019/10/29
 */

class IntegerKlass : public Klass {
public:
    static IntegerKlass *get_instance();

    virtual void print(PyObject *x);

    // TODO integer的以下运算都必须支持和double进行.
    virtual PyObject* add(PyObject *lhs, PyObject *rhs);
    virtual PyObject* sub(PyObject *lhs, PyObject *rhs);
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs);
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
