#include "PyInteger.hpp"
#include "VM.hpp"

#include <stdio.h>
#include <assert.h>

IntegerKlass::IntegerKlass(){}

IntegerKlass* IntegerKlass::m_Instance = nullptr;
std::mutex IntegerKlass::m_Mutex;

IntegerKlass* IntegerKlass::get_instance()
{
    if(nullptr == IntegerKlass::m_Instance)
    {
        std::lock_guard<std::mutex> lock(IntegerKlass::m_Mutex);
        IntegerKlass::m_Instance = new IntegerKlass();
    }
    return IntegerKlass::m_Instance;
}


void print(PyObject *x)
{
    assert(x->klass() == static_cast<Klass*>(this));
    printf("%d", dynamic_cast<PyInteger*>(x)->value());
}

PyObject* IntegerKlass::add(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    int x = dynamic_cast<PyInteger*>(lhs)->value();
    int y = dynamic_cast<PyInteger*>(rhs)->value();
    return new PyInteger(x + y);
}

PyObject* IntegerKlass::sub(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    int x = dynamic_cast<PyInteger*>(lhs)->value();
    int y = dynamic_cast<PyInteger*>(rhs)->value();
    return new PyInteger(x - y);
}

PyObject* IntegerKlass::mul(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    int x = dynamic_cast<PyInteger*>(lhs)->value();
    int y = dynamic_cast<PyInteger*>(rhs)->value();
    return new PyInteger(x * y);
}

PyObject* IntegerKlass::div(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    int x = dynamic_cast<PyInteger*>(lhs)->value();
    int y = dynamic_cast<PyInteger*>(rhs)->value();
    return new PyInteger(x / y);
}

PyObject* IntegerKlass::mod(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    assert(rhs && rhs->klass() == static_cast<Klass*>(this));
    int x = dynamic_cast<PyInteger*>(lhs)->value();
    int y = dynamic_cast<PyInteger*>(rhs)->value();
    return new PyInteger(x % y);
}

PyObject* IntegerKlass::greater(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() > dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* IntegerKlass::less(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() < dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
PyObject* IntegerKlass::equal(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() == dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* IntegerKlass::not_equal(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() != dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* IntegerKlass::ge(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() >= dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyObject* IntegerKlass::le(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() <= dynamic_cast<PyInteger*>(rhs))
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}
