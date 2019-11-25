#include "PyDict.hpp"
#include "PyInteger.hpp"
#include "PyDictIterator.hpp"

#include <iostream>
#include <cassert>

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

/**
 * 打印当前字典
 */
void DictKlass::print(PyObject *self)
{
    assert(self && self->klass() == this);
    PyDict *dict = dynamic_cast<PyDict*>(self);
    auto pMapEntries = dict->innerMap()->entries();
    printf("{");
    for(int i = 0; i < dict->size(); i++)
    {
        pMapEntries[i].m_K->print();
        printf(" : ");
        pMapEntries[i].m_V->print();
        if(i < dict->size() -1 )
        {
            printf(", ");
        }
    }
    printf("}");
    fflush(stdout);
}

PyObject* DictKlass::len(PyObject *self)
{
    assert(self && self->klass() == this);
    PyDict *slf = dynamic_cast<PyDict*>(self);
    return new PyInteger(slf->size());
}

PyObject* DictKlass::iter(PyObject *self)
{
    assert(self && self->klass() == this);
    PyDict *slf = dynamic_cast<PyDict*>(self);
    return new PyDictKeyIterator(slf);
}

PyObject* DictKlass::subscr(PyObject *self, PyObject *key)
{
    assert(self && self->klass() == this);
    PyDict *dict = dynamic_cast<PyDict*>(self);
    if(dict->contains(key))
    {
        return dict->get(key);
    }
    __throw_python_except("KeyError \n");
}

/**
 * 对字典进行下标赋值操作.
 */
PyObject* DictKlass::store_subscr(PyObject *self, PyObject *key, PyObject *value)
{
    assert(self && self->klass() == this);
    PyDict *dict = dynamic_cast<PyDict*>(self);
    // replace if key exisits and append if not exisits.
    dict->put(key, value);
    return VM::PyNone;
}

PyDict::PyDict()
{
    set_klass(DictKlass::get_instance());
    m_InnerMap = new Map<PyObject*, PyObject*>();
}

PyDict::PyDict(int init_cap)
{
    set_klass(DictKlass::get_instance());
    m_InnerMap = new Map<PyObject*, PyObject*>(8);
}

// copy一堆指针实际没啥用 没有做DeepCopy
// PyDict::PyDict(const PyDict &rhs)
// {
//     set_klass(DictKlass::get_instance());
//     m_InnerMap = new Map<PyObject*,PyObject*>(rhs.capacity());
//     // make copy
//     MapEntry<PyObject*, PyObject*> *thisEntry = 
//         const_cast<MapEntry<PyObject*, PyObject*> *>(m_InnerMap->entries());
//     const MapEntry<PyObject*, PyObject*> *rhsEntry = rhs.m_InnerMap->entries();
//     for(auto i = 0; i < rhs.m_InnerMap->size(); i++)
//     {
//         thisEntry[i] = rhsEntry[i];
//     }
// }

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