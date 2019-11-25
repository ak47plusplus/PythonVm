#ifndef PY_DICT_ITERATOR_HPP__
#define PY_DICT_ITERATOR_HPP__ 1

#include "Klass.hpp"
#include "Iterator.hpp"
#include "PyObject.hpp"

#include <mutex>

class PyDict;

MAKE_KLASS(DictKeyIteratorKlass)

/**
 * Python字典的Key迭代器
 */
class PyDictKeyIterator :public PyObject, public Iterator<PyObject*> {
public:
    PyDictKeyIterator(PyDict *target);
    virtual ~PyDictKeyIterator() = default;
    bool has_next();
    PyObject* next();
private:
    PyDict *m_Target;
    int     m_Cursor;
};

#endif