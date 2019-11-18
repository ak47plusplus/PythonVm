#include "PyObject.hpp"
#include "Klass.hpp"
#include "PyInteger.hpp"

PyObject *PyObject::id()
{
    #if __SIZEOF_POINTER__ == 8
        return new PyInteger((int)reinterpret_cast<int64_t>(this));
    #else
        return new PyInteger((int)reinterpret_cast<int32_t>(this));
    #endif
}

void PyObject::print()
{
    this->m_Klass->print(this);
}

// 这里强行约定 lhs全部用this.

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

PyObject *PyObject::len()
{
    return this->m_Klass->len(this);
}

PyObject *PyObject::subscr(PyObject *rhs)
{
    return this->m_Klass->subscr(this, rhs);
}

PyObject *PyObject::store_subscr(PyObject *mhs, PyObject *rhs)
{   
    return this->m_Klass->store_subscr(this, mhs, rhs);
}

PyObject *PyObject::del_subscr(PyObject *rhs)
{
    return this->m_Klass->del_subscr(this, rhs);
}

PyObject *PyObject::contains(PyObject *rhs)
{
    return this->m_Klass->contains(this, rhs);
}

PyObject *PyObject::getattr(PyObject *attrK)
{
    return this->m_Klass->getattr(this, attrK);
}
