#include "PyInteger.hpp"
#include <iostream>

void PyInteger::print()
{
    std::cout << this->m_value ;
}

PyObject* PyInteger::add(PyObject *rhs)
{
    return new PyInteger(this->m_value + dynamic_cast<PyInteger*>(rhs)->m_value);
}

PyObject* PyInteger::greater(PyObject *rhs)
{
    if (this->m_value > dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::less(PyObject *rhs)
{
    if (this->m_value < dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::equal(PyObject *rhs)
{
    if (this->m_value == dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::not_equal(PyObject *rhs)
{
    if (this->m_value != dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::ge(PyObject *rhs)
{
    if (this->m_value >= dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::le(PyObject *rhs)
{
    if (this->m_value <= dynamic_cast<PyInteger*>(rhs)->m_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}
