#ifndef PY_LIST_ITERATOR_HPP
#define PY_LIST_ITERATOR_HPP

#include "Klass.hpp"
#include "PyObject.hpp"
#include "Iterator.hpp"

#include <mutex>

class PyList;

class ListIteratorKlass : public Klass {
public:
    static ListIteratorKlass* get_instance();
private:
    static ListIteratorKlass* m_Instance;
    static std::mutex         m_Mutex;
    ListIteratorKlass();
};

class PyListIterator : public PyObject, public Iterator<PyObject*> {
public:
    PyListIterator(PyList *list);
    virtual ~PyListIterator() = default;
    virtual bool has_next();
    virtual PyObject* next();
private:
    PyList *m_Target;   // do not hold the ownership.
    int     m_Cursor;
};

#endif 