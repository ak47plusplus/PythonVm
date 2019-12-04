#include "PyDict.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyDictIterator.hpp"
#include "PyFunction.hpp"
#include "ArrayList.hpp"
#include "VM.hpp"
#include <iostream>
#include <cassert>

DictKlass* DictKlass::m_Instance = nullptr;
std::mutex DictKlass::m_Mutex;

DictKlass::DictKlass()
{
    set_name("dict");
}

void DictKlass::InitKlass()
{
    PyDict *attrs = new PyDict(8);
    attrs->put(new PyString("clear"), new PyFunction(pydict::dict_clear));
    set_attrs(attrs);
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
    auto *slf = dynamic_cast<PyDict*>(self);
    return new PyInteger(slf->size());
}

PyObject* DictKlass::iter(PyObject *self)
{
    assert(self && self->klass() == this);
    auto *slf = dynamic_cast<PyDict*>(self);
    return new PyDictKeyIterator(slf);
}

PyObject* DictKlass::subscr(PyObject *self, PyObject *key)
{
    assert(self && self->klass() == this);
    auto *dict = dynamic_cast<PyDict*>(self);
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
    auto *dict = dynamic_cast<PyDict*>(self);
    // replace if key exisits and append if not exisits.
    dict->put(key, value);
    return VM::PyNone;
}

/**
 * 对字典进行删除下标操作
 * <p>
 * e.g: 
 * score = {'zhangsan':98, "lisi":67}
 * del score['lisi']
 */
PyObject* DictKlass::del_subscr(PyObject *self, PyObject *key)
{
    assert(self && self->klass() == this);
    auto *dict = dynamic_cast<PyDict*>(self);
    if(dict->contains(key))
        dict->del_key(key);
    else 
        __throw_python_except("KeyError \n");
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

PyDict::PyDict(PyDict &&rhs) NOEXCEPT
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


namespace pydict
{

/**
 * 清空整个Dict.
 */
PyObject *dict_clear(FuncArgs args)
{
    assert(args && args->size() == 1);
    PyDict *dict = dynamic_cast<PyDict*>(args->get(0));
    dict->clear();
    return VM::PyNone;
}

/**
 * 返回key组成的list
 * Note：Python2返回的是list 但是python3返回的是dict_keys类型,这里只兼容python2
 */
PyObject* dict_keys(FuncArgs args)
{
    assert(args && args->size() == 1);
    PyDict *dict = dynamic_cast<PyDict*>(args->get(0));
    InnerMapPtr innerMap = dict->innerMap();
    PyList *keys = new PyList();
    for(auto i = 0; i < innerMap->size(); i++)
    {
        keys->append(innerMap->get_key(i));
    }
    return keys;
}

/**
 * 返回value组成的list
 * Note：Python2返回的是list 但是python3返回的是dict_keys类型,这里只兼容python2
 */
PyObject* dict_values(FuncArgs args)
{
    assert(args && args->size() == 1);
    PyDict *dict = dynamic_cast<PyDict*>(args->get(0));
    InnerMapPtr innerMap = dict->innerMap();
    const MapEntry<PyObject*,PyObject*> *entries = innerMap->entries();
    auto size = innerMap->size();
    PyList *values = new PyList();
    for(auto i = 0; i < size; i++)
    {
        values->append(entries[i].m_V);
    }
    return values;
}

} // namespace pydict
