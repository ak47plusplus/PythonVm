#ifndef PY_STRING_ITERATOR_HPP_
#define PY_STRING_ITERATOR_HPP_

#include "Klass.hpp"
#include "Iterator.hpp"
#include "PyObject.hpp"
#include <mutex>

MAKE_KLASS(StringIteratorKlass)

class PyString;
class PyStringIterator : public PyObject, public Iterator<PyObject*> {
public:
    PyStringIterator(PyString *pyStr);
    virtual ~PyStringIterator() = default;
    virtual bool has_next();
    virtual PyObject* next();
private:
    PyString *m_Target;     // do not hold the ownership.
    int       m_Cursor;
};

#endif