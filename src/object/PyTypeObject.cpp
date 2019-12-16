//
// Created by guoxiang on 2019/11/30.
//

#include "PyTypeObject.hpp"
#include "PyDict.hpp"

std::mutex TypeKlass::m_Mutex;
TypeKlass* TypeKlass::m_Instance = nullptr;

TypeKlass::TypeKlass()
{
    set_name("type");
    set_attrs(new PyDict());
}

TypeKlass *TypeKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new TypeKlass();
        }
    }
    return m_Instance;
}

