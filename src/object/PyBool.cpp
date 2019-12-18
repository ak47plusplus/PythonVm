#include "PyBool.hpp"
#include "PyDict.hpp"
#include "PyTypeObject.hpp"

#include <cassert>
#include <iostream>

std::unique_ptr<BoolKlass> BoolKlass::m_Instance;
std::mutex BoolKlass::m_Mutex;

BoolKlass::BoolKlass()
{
    set_name("bool");
}

void BoolKlass::InitKlass()
{
    set_attrs(new PyDict());
    set_super(ObjectKlass::get_instance());
    (new PyTypeObject())->set_own_klass(this);
}

BoolKlass* BoolKlass::get_instance()
{
    if(!m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(!m_Instance)
        {
            m_Instance.reset(new BoolKlass());
        }
    }
    return m_Instance.get();
}

void BoolKlass::print(PyObject *x)
{
    assert(x && x->klass() == this);
    PyBool *pBool = dynamic_cast<PyBool*>(x);
    std::cout << pBool->value();
    std::fflush(stdout);
}

PyBool::PyBool()
{
    set_klass(BoolKlass::get_instance());
    m_Value = false;
}

PyBool::PyBool(bool v)
:   m_Value(v)
{
    set_klass(BoolKlass::get_instance());
}