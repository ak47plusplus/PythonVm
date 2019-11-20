#include "PyDict.hpp"

#include <iostream>

DictKlass* DictKlass::m_Instance = nullptr;
std::mutex DictKlass::m_Mutex;

DictKlass::DictKlass()
{
    set_name("dict");
}

DictKlass* DictKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new DictKlass();
        }
    }
    return m_Instance;
}

PyDict::PyDict()
{
    set_klass(DictKlass::get_instance());
    m_InnerMap = new Map<PyObject*, PyObject*>();
}

// copy一堆指针实际没啥用 没有做DeepCopy
PyDict::PyDict(const PyDict &rhs)
{
    set_klass(DictKlass::get_instance());
    m_InnerMap = new Map<PyObject*,PyObject*>(rhs.capacity());
    // make copy
    for(auto i = 0; i < rhs.m_InnerMap->size(); i++)
    {
        (*m_InnerMap)[i] = (*(rhs.m_InnerMap))[i];
    }
}

PyDict::PyDict(PyDict &&rhs)
{
    set_klass(DictKlass::get_instance());
    // move the owership
    m_InnerMap = rhs.m_InnerMap;
    rhs.m_InnerMap = nullptr;
}

PyDict::~PyDict()
{
    if(nullptr != m_InnerMap)
    {
        delete m_InnerMap;
        m_InnerMap = nullptr;
    }
}