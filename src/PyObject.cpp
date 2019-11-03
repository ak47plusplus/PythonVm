#include "PyObject.hpp"
#include "Klass.hpp"

void PyObject::print()
{
    this->m_Klass->print(this);
}

PyObject *PyObject::add(PyObject *rhs)
{
    return this->m_Klass->add(this, rhs);
}

PyObject *PyObject::sub(PyObject *rhs)
{
    return this->m_Klass->sub(this, rhs);
}

PyObject *PyObject::mul(PyObject *rhs)
{
    return this->m_Klass->mul(this, rhs);
}

PyObject *PyObject::div(PyObject *rhs)
{
    return this->m_Klass->div(this, rhs);
}

PyObject *PyObject::mod(PyObject *rhs)
{
    return this->m_Klass->mod(this, rhs);
}

PyObject *PyObject::greater(PyObject *rhs)
{
    return this->m_Klass->greater(this, rhs);
}

PyObject *PyObject::less(PyObject *rhs)
{
    return this->m_Klass->less(this, rhs);
}

PyObject *PyObject::equal(PyObject *rhs)
{
    return this->m_Klass->equal(this, rhs);
}

PyObject *PyObject::not_equal(PyObject *rhs)
{
    return this->m_Klass->not_equal(this, rhs);
}

PyObject *PyObject::ge(PyObject *rhs)
{
    return this->m_Klass->ge(this, rhs);
}

PyObject *PyObject::le(PyObject *rhs)
{
    return this->m_Klass->le(this, rhs);
}