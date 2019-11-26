#ifndef PY_TUPLE_HPP_
#define PY_TUPLE_HPP_ 1

#include "Klass.hpp"
#include "PyObject.hpp"
#include "ArrayList.hpp"

#include <mutex>

// what the difference between list and tuple ?
// en... why we need this ?
// i dont known...

class TupleKlass : public Klass {
public:
    static TupleKlass *get_instance();

private:
    TupleKlass();
    static TupleKlass *m_Instance;
    static std::mutex  m_Mutex;
};

class PyTuple : public PyObject {
public:
    PyTuple();
    PyTuple(const PyTuple &rhs);
    PyTuple(PyTuple &&rhs);
    ~PyTuple();
private:
    ArrayList<PyObject*> *m_InnerContainer;
};



#endif