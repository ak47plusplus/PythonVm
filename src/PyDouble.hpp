#ifndef PY_DOUBLE_HPP___
#define PY_DOUBLE_HPP___

#include <mutex>
#include "Klass.hpp"
#include "PyObject.hpp"


class DoubleKlass : public Klass {
public:
    static DoubleKlass *get_instance();

    virtual void print(PyObject *x);

    // TODO double的以下操作都支持和integer进行运算.
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
    DoubleKlass();
    static DoubleKlass *m_Instance;
    static std::mutex   m_Mutex;
};


class PyDouble : public PyObject {
public:
    PyDouble(double val);
    double value() {return m_Value;}
private:
    double m_Value;
};

#endif
