#include "Klass.hpp"
#include "PyString.hpp"
#include "Panic.hpp"

Klass::Klass()
{
    this->m_Name = nullptr;
}

Klass::~Klass()
{
    if(m_Name != nullptr) delete m_Name;
}

void Klass::register_klass_dict(PyObject* k, PyObject *v)
{
    auto ret = m_KlassDict.insert(std::pair<PyObject*,PyObject*>(k,v));
    if(ret.second == false)
        __panic("register klass dict failed !\n");
}


PyObject* Klass::getattr(PyObject *x, PyObject *y)
{
    return m_KlassDict[y];
}