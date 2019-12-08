#ifndef PY_LIST_HPP__
#define PY_LIST_HPP__

#include "Klass.hpp"
#include "PyObject.hpp"
#include "ArrayList.hpp"
#include <mutex>

class ListKlass : public Klass {
public:
    static ListKlass *get_instance();
    void InitKlass() override;

    void print(PyObject *x) override;
    PyObject *len(PyObject *self) override;
    PyObject *iter(PyObject *self) override;

    PyObject* add(PyObject *lhs, PyObject *rhs) override;
    PyObject* mul(PyObject *lhs, PyObject *rhs) override;

    PyObject *subscr(PyObject *lhs, PyObject *rhs) override;
    PyObject *store_subscr(PyObject *lhs, PyObject *mhs, PyObject *rhs) override;
    PyObject *del_subscr(PyObject *lhs, PyObject *rhs) override;
    PyObject *contains(PyObject *lhs, PyObject *rhs) override;

private:
    ListKlass();
    static ListKlass *m_Instance;
    static std::mutex m_Mutex;
};

class PyList : public PyObject {
public:
    PyList();
    explicit PyList(ArrayList<PyObject*> *list);
    PyList(const PyList &rhs);
    PyList(PyList &&rhs) NOEXCEPT;
    PyList& operator=(const PyList &rhs);
    ~PyList() override;

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
    void reverse()                        { m_InnerList->reverse(); }

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
    extern PyObject *list_sort(FuncArgs args);
    extern PyObject *list_reverse(FuncArgs args);
}

#endif
