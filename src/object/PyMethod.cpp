#include "PyMethod.hpp"

std::mutex MethodKlass::m_Mutex;
MethodKlass *MethodKlass::m_Instance = nullptr;

MethodKlass::MethodKlass()
{
    set_name("method");
}

MethodKlass *MethodKlass::get_instance()
{
    if (nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new MethodKlass();
        }
    }
    return m_Instance;
}

PyMethod::PyMethod(PyFunction *func)
    : m_Owner(nullptr), m_Func(func)
{
    set_klass(MethodKlass::get_instance();)
}

PyMethod::PyMethod(PyObject *owner, PyFunction *func)
    : m_Owner(owner), m_Func(func)
{
    set_klass(MethodKlass::get_instance();)
}

bool MethodObject::is_function(PyObject *obj)
{
    return obj && (
        obj->klass() == MethodKlass::get_instance() ||
        obj->klass() == FunctionKlass::get_instance();
    )
}
bool MethodObject::is_yield_function(PyObject *obj)
{
    return false;
}
