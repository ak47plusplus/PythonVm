#include "PyDouble.hpp"
#include "VM.hpp"
#include <stdio.h>
#include <assert.h>

DoubleKlass::DoubleKlass()
{

}

DoubleKlass *DoubleKlass::m_Instance = nullptr;
std::mutex   DoubleKlass::m_Mutex;

DoubleKlass *DoubleKlass::get_instance()
{
    if(nullptr == DoubleKlass::m_Instance)
    {
        std::lock_guard<std::mutex> lock(DoubleKlass::m_Mutex);
        if(nullptr == DoubleKlass::m_Instance)
        {
            DoubleKlass::m_Instance = new DoubleKlass();
        }
    }
    return DoubleKlass::m_Instance;
}

void DoubleKlass::print(PyObject *x)
{
    assert(x->klass() == static_cast<Klass*>(this));
    printf("%f", dynamic_cast<PyDouble*>(x)->value());
}

PyObject* DoubleKlass::add(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    PyDouble *dl = dynamic_cast<PyDouble*>(lhs);
    PyDouble *dr = dynamic_cast<PyDouble*>(rhs);
    return new PyDouble(dl->value() + dr->value());
}
PyObject* DoubleKlass::sub(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    PyDouble *dl = dynamic_cast<PyDouble*>(lhs);
    PyDouble *dr = dynamic_cast<PyDouble*>(rhs);
    return new PyDouble(dl->value() - dr->value());
}
PyObject* DoubleKlass::mul(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    PyDouble *dl = dynamic_cast<PyDouble*>(lhs);
    PyDouble *dr = dynamic_cast<PyDouble*>(rhs);
    return new PyDouble(dl->value() * dr->value());
}
PyObject* DoubleKlass::div(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    PyDouble *dl = dynamic_cast<PyDouble*>(lhs);
    PyDouble *dr = dynamic_cast<PyDouble*>(rhs);
    return new PyDouble(dl->value() / dr->value());
}

PyObject* DoubleKlass::mod(PyObject *lhs, PyObject *rhs)
{
    return new PyDouble(0.0d);
}

PyObject* DoubleKlass::greater(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() > dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* DoubleKlass::less(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() < dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* DoubleKlass::equal(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() == dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* DoubleKlass::not_equal(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() != dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* DoubleKlass::ge(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() >= dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* DoubleKlass::le(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyDouble*>(lhs)->value() <= dynamic_cast<PyDouble*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

// == for DoubleObject.

PyDouble::PyDouble(double val) : m_Value(val) {
    set_klass(DoubleKlass::get_instance());
}
