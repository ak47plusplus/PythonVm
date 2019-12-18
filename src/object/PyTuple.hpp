#ifndef PY_TUPLE_HPP_
#define PY_TUPLE_HPP_ 1

#include "Klass.hpp"
#include "PyObject.hpp"
#include "ArrayList.hpp"
#include "PyNoneType.hpp"

#include <mutex>

// what the difference between list and tuple ?
// en... why we need this ?
// i dont known...

class TupleKlass : public Klass {
public:
    static TupleKlass *get_instance();
    void InitKlass() override;
    void print(PyObject *x) override;
    PyObject* add(PyObject *lhs, PyObject *rhs) override;
private:
    TupleKlass();
    static TupleKlass *m_Instance;
    static std::mutex  m_Mutex;
};

class PyTuple : public PyObject {
public:
    PyTuple();
    PyTuple(ArrayList<PyObject*> *list);
    PyTuple(PyTuple &&rhs) NOEXCEPT;
    PyTuple(const PyTuple &rhs) = delete;
    PyTuple& operator=(const PyTuple &rhs) = delete;
    ~PyTuple() override;

    // PyTuple operator+(const PyTuple &rhs);
    int size()               { return m_InnerContainer->size(); }
    PyObject* get(int index) { return m_InnerContainer->get(index);}
    void set(int index, PyObject *obj) { m_InnerContainer->set(index, obj);}
    void _add(PyObject *unsafeE) { m_InnerContainer->add(unsafeE); }
private:
    ArrayList<PyObject*> *m_InnerContainer;
};

#endif