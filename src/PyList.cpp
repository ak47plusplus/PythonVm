#include "PyList.hpp"

ListKlass* ListKlass::m_Instance = nullptr;
std::mutex ListKlass::m_Mutex;

ListKlass::ListKlass()
{}

ListKlass *ListKlass::get_instance()
{
    if (nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = nullptr;
        }
    }
    return m_Instance;
}

void ListKlass::print(PyObject *x)
{
    
}