#include "PyNoneType.hpp"
#include "PyTypeObject.hpp"
#include "PyDict.hpp"

#include <cassert>
#include <iostream>

std::mutex NoneTypeKlass::m_Mutex;
std::unique_ptr<NoneTypeKlass> NoneTypeKlass::m_Instance;

NoneTypeKlass::NoneTypeKlass()
{
    set_name("NoneType");
}

void NoneTypeKlass::InitKlass()
{
    set_attrs(new PyDict());
    set_super(ObjectKlass::get_instance());
    (new PyTypeObject())->set_own_klass(this);
}

NoneTypeKlass* NoneTypeKlass::get_instance()
{
    if(!m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(!m_Instance)
        {
            m_Instance.reset(new NoneTypeKlass());
        }
    }
    return m_Instance.get();
}

void NoneTypeKlass::print(PyObject *x)
{
    assert(x && x->klass() == this);
    std::cout << "None";
    std::fflush(stdout);
}
