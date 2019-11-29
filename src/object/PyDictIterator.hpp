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
    explicit PyDictKeyIterator(PyDict *target);
    ~PyDictKeyIterator() override = default;
    bool has_next() override;
    PyObject* next() override;
private:
    PyDict *m_Target;
    int     m_Cursor;
};

#endif