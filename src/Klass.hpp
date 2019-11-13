#ifndef KLASS_HPP___
#define KLASS_HPP___

// Forward declaration
class PyString;
class PyObject;
class PyList;
class PyDict;

class Klass {
public:
    Klass();
    ~Klass();
    void set_name(PyString *name) {this->m_Name = name;}
    PyString *name()              { return this->m_Name;}

    virtual void print(PyObject *x)                          {}

    virtual PyObject* add(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* sub(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* div(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* mod(PyObject *lhs, PyObject *rhs)      {return 0;}


    virtual PyObject* greater(PyObject *lhs, PyObject *rhs)  {return 0;}
    virtual PyObject* less(PyObject *lhs, PyObject *rhs)     {return 0;}
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs)    {return 0;}
    virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs){return 0;}
    virtual PyObject* ge(PyObject *lhs, PyObject *rhs)       {return 0;}
    virtual PyObject* le(PyObject *lhs, PyObject *rhs)       {return 0;}

    virtual PyObject* len(PyObject *x)                       {return 0;}

private:
    PyString *m_Name;
};

#endif
