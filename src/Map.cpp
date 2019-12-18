#include "Map.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyNoneType.hpp"
#include "PyBool.hpp"

// #include <iostream>

// 针对k-v都是PyObject*的特化版本

typedef PyObject *pPyObject;

template<>
void Map<pPyObject,pPyObject>::put(const pPyObject &k, const pPyObject &v)
{
    // std::cout << "Map::put specicial version..." << std::endl;
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K->equal(k) == VM::PyTrue)
        {
            m_Entries[i].m_V = v;
            return;
        }
    }
    if(m_Size >= m_Capacity)
    {
        this->expand_capacity();
    }
    m_Entries[m_Size++] = MapEntry<pPyObject,pPyObject>(k,v);
}

template<>
int Map<pPyObject,pPyObject>::index(const pPyObject &k)
{
    // std::cout << "Map::index specicial version..." << std::endl;
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K->equal(k) == VM::PyTrue)
        {
            return i;
        }
    }
    return -1;
}