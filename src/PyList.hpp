#ifndef PY_LIST_HPP__
#define PY_LIST_HPP__

#include "Klass.hpp"
#include "PyObject.hpp"
#include "ArrayList.hpp"
#include <mutex>

class ListKlass : public Klass {
public:
    virtual void print(PyObject *x);
    virtual PyObject *subscr(PyObject *lhs, PyObject *rhs);
    virtual PyObject *contains(PyObject *lhs, PyObject *rhs);

    static ListKlass *get_instance();
private:
    ListKlass();
    static ListKlass *m_Instance;
    static std::mutex m_Mutex;
};

class PyList : public PyObject {
public:
    PyList();
    PyList(ArrayList<PyObject*> *list);
    PyList(const PyList &rhs);
    PyList(PyList &&rhs);
    PyList& operator=(const PyList &rhs);
    ~PyList();

    ArrayList<PyObject*>* inner_list()    { return m_InnerList;}

    int size()     const                  { return m_InnerList->size();}
    void append(PyObject *obj)            { m_InnerList->add(obj);}
    PyObject *pop()                       { return m_InnerList->pop();}
    PyObject *get(int index)              { return m_InnerList->get(index);}
    void set(int index, PyObject *obj)    { m_InnerList->set(index, obj);}
    void insert(int index, PyObject *obj) { m_InnerList->insert(index, obj);}
    PyObject *top()                       { return m_InnerList->get(this->size() - 1);}

    int index(PyObject *obj)              { return m_InnerList->index(obj);}
    void delete_index(int index)          { m_InnerList->delete_index(index);}
    void clear()                          { m_InnerList->clear();}
    int count(PyObject *target);

private:
    ArrayList<PyObject*> *m_InnerList;
};

namespace pylist {
    typedef ArrayList<PyObject*> *FuncArgs;

    extern PyObject *list_append(FuncArgs args);
    extern PyObject *list_insert(FuncArgs args);
    extern PyObject *list_index(FuncArgs args);
    extern PyObject *list_pop(FuncArgs args);
    extern PyObject *list_remove(FuncArgs args);
    extern PyObject *list_clear(FuncArgs args);
    extern PyObject *list_count(FuncArgs args);
}

#endif
