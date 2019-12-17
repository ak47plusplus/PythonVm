#include "ConstantsPool.hpp"

ConstantsPool* ConstantsPool::m_Instance = nullptr;
std::mutex ConstantsPool::m_Mutex;

ConstantsPool& ConstantsPool::Instance()
{
    if(m_Instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(m_Instance == nullptr)
        {
            m_Instance = new ConstantsPool();
        }
    }
    return *m_Instance;
}

ConstantsPool::ConstantsPool()
{
    _MODULE_ = new PyString("__module__");
}

ConstantsPool::~ConstantsPool()
{

}