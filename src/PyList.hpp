#ifndef PY_LIST_HPP__
#define PY_LIST_HPP__

#include "PyObject.hpp"
#include "ArrayList.hpp"

/**
 *  @brief Python的list可以装任何类型元素.
 *  @Created by GuoXiang 2019/11/1
 */
class PyList : public PyObject {
public:
    PyList();
    ~PyList();
    PyList(const PyList &rhs);
    PyList& operator=(const PyList &rhs);

    ArrayList<PyObject*>* inner_list()    { return m_InnerList;}

    int size()                            { return m_InnerList->size();}
    void append(PyObject *obj)            { m_InnerList->add(obj);}
    PyObject *pop()                       { return m_InnerList->pop();}
    PyObject *get(int index)              { return m_InnerList->get(index);}
    void set(int index, PyObject *obj)    { m_InnerList->set(index, obj);}
    PyObject *top()                       { return m_InnerList->get(this->size() - 1);}

    int index(PyObject *obj);
    void delete_index(int index)          { m_InnerList.delete_index(index);}

private:
    ArrayList<PyObject*> *m_InnerList;
};

#endif
