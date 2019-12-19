#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include "PyString.hpp"
#include "PyDict.hpp"
#include "PyTypeObject.hpp"
#include "VM.hpp"
#include "Panic.hpp"
#include "Python.hpp"

#include <stdio.h>
#include <math.h>   // fmod
#include <assert.h>

IntegerKlass::IntegerKlass()
{
    set_name("int");
}

void IntegerKlass::InitKlass()
{
    set_attrs(new PyDict());
    set_super(ObjectKlass::get_instance());
    (new PyTypeObject())->set_own_klass(this);
}

IntegerKlass* IntegerKlass::m_Instance = nullptr;
std::mutex IntegerKlass::m_Mutex;

IntegerKlass* IntegerKlass::get_instance()
{
    if(nullptr == IntegerKlass::m_Instance)
    {
        std::lock_guard<std::mutex> lock(IntegerKlass::m_Mutex);
            if(nullptr == IntegerKlass::m_Instance) {
                  IntegerKlass::m_Instance = new IntegerKlass();
            }
    }
    return IntegerKlass::m_Instance;
}

/**
 *  a = int()
 *  b = int(3)
 *  the typeobject will be called as a function.
 */
PyObject *IntegerKlass::allocate_instance(Klass::FuncArgs args)
{
    if(!args || args->size() == 0)
        return new PyInteger(0);
    else if(args->size() == 1)
    {
        if(!PyObject_Klass_In(args->get(0), 
            Klass_ToKlassPtr(IntegerKlass::get_instance()),
            Klass_ToKlassPtr(DoubleKlass::get_instance()),
            Klass_ToKlassPtr(StringKlass::get_instance())))
        {
            __throw_python_except("only integer and double are supported.\n");
        }
        return new PyInteger(99);
    } 
    else 
    {
        __throw_python_except("IntegerKlass::allocate_instance failed\n");
    }
}

void IntegerKlass::print(PyObject *x)
{
    assert(x->klass() == static_cast<Klass*>(this));
    printf("%d", dynamic_cast<PyInteger*>(x)->value());
}

PyObject* IntegerKlass::add(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    Klass * rKlass = rhs->klass();
    if(rKlass == nullptr)
    {
        __panic("rhs klass nullptr !");
    } else if( rKlass == IntegerKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyInteger *r = dynamic_cast<PyInteger*>(rhs);
        return new PyInteger(l->value() + r->value());
    } else if( rKlass == DoubleKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyDouble  *r = dynamic_cast<PyDouble*>(rhs);
        return new PyDouble(l->value() + r->value());
    } else {
        __panic("TypeError: Unsupported operand type(s) for +: 'int' and '?'\n");
    }
}

PyObject* IntegerKlass::sub(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    Klass * rKlass = rhs->klass();
    if(rKlass == nullptr)
    {
        __panic("rhs klass nullptr !");
    } else if( rKlass == IntegerKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyInteger *r = dynamic_cast<PyInteger*>(rhs);
        return new PyInteger(l->value() - r->value());
    } else if( rKlass == DoubleKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyDouble  *r = dynamic_cast<PyDouble*>(rhs);
        return new PyDouble(l->value() - r->value());
    } else {
        __panic("TypeError: Unsupported operand type(s) for +: 'int' and '?'\n");
    }
}

PyObject* IntegerKlass::mul(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    Klass * rKlass = rhs->klass();
    if(rKlass == nullptr)
    {
        __panic("rhs klass nullptr !");
    } else if( rKlass == IntegerKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyInteger *r = dynamic_cast<PyInteger*>(rhs);
        return new PyInteger(l->value() * r->value());
    } else if( rKlass == DoubleKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyDouble  *r = dynamic_cast<PyDouble*>(rhs);
        return new PyDouble(l->value() * r->value());
    } else if( rKlass == StringKlass::get_instance()) {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyString *r  = dynamic_cast<PyString*>(rhs);
        return PyString::times(r,l->value());
    } else{
        __panic("TypeError: Unsupported operand type(s) for +: 'int' and '?'\n");
    }
}

PyObject* IntegerKlass::div(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    Klass * rKlass = rhs->klass();
    if(rKlass == nullptr)
    {
        __panic("rhs klass nullptr !");
    } else if( rKlass == IntegerKlass::get_instance())
    {
        // python的除法比较坑爹 两个整数无论是否除尽都是浮点数
        // 3/1 = 3.0  3/2=1.5
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyInteger *r = dynamic_cast<PyInteger*>(rhs);
        if(r->value() == 0) {
            __panic("div by zero !");
        }
        return new PyDouble(((double)(l->value())) / ((double)(r->value())));
    } else if( rKlass == DoubleKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyDouble  *r = dynamic_cast<PyDouble*>(rhs);
        if(r->value() == 0) {
            // __panic("div by zero !"); 除数为浮点数0 结果为Infinity
        }
        return new PyDouble(l->value() / r->value());
    } else {
        __panic("TypeError: Unsupported operand type(s) for +: 'int' and '?'\n");
    }
}

PyObject* IntegerKlass::mod(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == static_cast<Klass*>(this));
    Klass * rKlass = rhs->klass();
    if(rKlass == nullptr)
    {
        __panic("rhs klass nullptr !");
    } else if( rKlass == IntegerKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyInteger *r = dynamic_cast<PyInteger*>(rhs);
        return new PyInteger(l->value() % r->value());
    } else if( rKlass == DoubleKlass::get_instance())
    {
        PyInteger *l = dynamic_cast<PyInteger*>(lhs);
        PyDouble  *r = dynamic_cast<PyDouble*>(rhs);
        // return new PyDouble(((double)(l->value())) % r->value());
        return new PyDouble(fmod(((double)(l->value())), r->value()));
    } else {
        __panic("TypeError: Unsupported operand type(s) for +: 'int' and '?'\n");
    }
}

PyObject* IntegerKlass::greater(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    if(dynamic_cast<PyInteger*>(lhs)->value() > dynamic_cast<PyInteger*>(rhs)->value())
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

    if(dynamic_cast<PyInteger*>(lhs)->value() < dynamic_cast<PyInteger*>(rhs)->value())
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

    if(dynamic_cast<PyInteger*>(lhs)->value() == dynamic_cast<PyInteger*>(rhs)->value())
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

    if(dynamic_cast<PyInteger*>(lhs)->value() != dynamic_cast<PyInteger*>(rhs)->value())
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

    if(dynamic_cast<PyInteger*>(lhs)->value() >= dynamic_cast<PyInteger*>(rhs)->value())
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

    if(dynamic_cast<PyInteger*>(lhs)->value() <= dynamic_cast<PyInteger*>(rhs)->value())
    {
        return VM::PyTrue;
    } else {
        return VM::PyFalse;
    }
}

PyInteger::PyInteger(int x)
{
    m_Value = x;
    set_klass(IntegerKlass::get_instance());
}
