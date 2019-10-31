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
