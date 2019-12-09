#include "PyTuple.hpp"

#include <cstdio>
#include <cassert>

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

void TupleKlass::print(PyObject *x)
{
    assert(PyObject_Klass_Check(x, this));
    auto * tuple = dynamic_cast<PyTuple*>(x);
    printf("%s","(");
    auto tupleSize = tuple->size();
    for(auto i = 0; i < tupleSize; i++)
    {
        tuple->get(i)->print();
        if(i < tupleSize -1)
        {
            printf(",");
        }
    }
    printf("%s", ")");
    std::fflush(stdout);
}

PyObject* TupleKlass::add(PyObject *lhs, PyObject *rhs)
{
    assert(PyObject_Klass_Check(lhs, this));
    if(!PyObject_Klass_Check(rhs, this))
    {
        __throw_python_except("TypeError: can only concatenate tuple to tuple.\n");
    }
    auto first = dynamic_cast<PyTuple*>(lhs);
    auto second = dynamic_cast<PyTuple*>(rhs);
    PyTuple *unionTuple = new PyTuple();
    for(auto i = 0; i < first->size(); i++) 
    {
        unionTuple->_add(first->get(i));
    }
    for(auto i = 0; i < second->size(); i++) 
    {
        unionTuple->_add(second->get(i));
    }
    return unionTuple;
}

PyTuple::PyTuple()
{
    m_InnerContainer = new ArrayList<PyObject*>(2);
    set_klass(TupleKlass::get_instance());
}

PyTuple::PyTuple(ArrayList<PyObject*> *list)
{
    m_InnerContainer = list;
    set_klass(TupleKlass::get_instance());
}

PyTuple::PyTuple(PyTuple &&rhs) NOEXCEPT
{
    set_klass(TupleKlass::get_instance());
    m_InnerContainer = rhs.m_InnerContainer;
    rhs.m_InnerContainer = nullptr;
}

PyTuple::~PyTuple()
{
    if(m_InnerContainer)
        delete m_InnerContainer;
}