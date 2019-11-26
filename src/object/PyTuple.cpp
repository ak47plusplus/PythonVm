#include "PyTuple.hpp"

TupleKlass* TupleKlass::m_Instance = nullptr;
std::mutex  TupleKlass::m_Mutex;

TupleKlass::TupleKlass()
{
    set_name("tuple");
}

TupleKlass *TupleKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new TupleKlass();
        }
    }
    return m_Instance;
}

