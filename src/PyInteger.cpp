#include "PyInteger.hpp"
#include <iostream>
#include "VM.hpp"

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
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* PyInteger::less(PyObject *rhs)
{
    if (this->m_value < dynamic_cast<PyInteger*>(rhs)->m_value) {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* PyInteger::equal(PyObject *rhs)
{
    if (this->m_value == dynamic_cast<PyInteger*>(rhs)->m_value) {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* PyInteger::not_equal(PyObject *rhs)
{
    if (this->m_value != dynamic_cast<PyInteger*>(rhs)->m_value) {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* PyInteger::ge(PyObject *rhs)
{
    if (this->m_value >= dynamic_cast<PyInteger*>(rhs)->m_value) {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* PyInteger::le(PyObject *rhs)
{
    if (this->m_value <= dynamic_cast<PyInteger*>(rhs)->m_value) {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
