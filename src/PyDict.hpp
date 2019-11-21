#ifndef PY_DICT_HPP__
#define PY_DICT_HPP__

#include "Map.hpp"
#include "Klass.hpp"
#include "PyObject.hpp"

#include <mutex>

class DictKlass : public Klass {
public:
    static DictKlass* get_instance();

    virtual void print(PyObject *self);

private:
    static DictKlass     *m_Instance;
    static std::mutex    m_Mutex;
    DictKlass();
};

class PyDict : public PyObject {
public:
    PyDict();
    PyDict(int init_cap = 8);
    PyDict(const PyDict &rhs) = delete;
    PyDict(PyDict &&rhs);
    ~PyDict();
    PyDict& operator=(const PyDict &rhs) = delete;

    void put(PyObject *first, PyObject *second) { m_InnerMap->put(first, second); }

    int size() const      { return m_InnerMap->size();}
    int capacity() const  { return m_InnerMap->capacity();}
    Map<PyObject*,PyObject*> *innerMap() { return m_InnerMap; }
private:
    Map<PyObject*,PyObject*> *m_InnerMap;
};

#endif
