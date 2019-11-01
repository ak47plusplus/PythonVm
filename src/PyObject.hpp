#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

class PyObject {
public:
    virtual ~PyObject() {}

    virtual void print() {}

    virtual PyObject* add(PyObject *rhs)      {return 0;}
    virtual PyObject* greater(PyObject *rhs)  {return 0;}
    virtual PyObject* less(PyObject *rhs)     {return 0;}
    virtual PyObject* equal(PyObject *rhs)    {return 0;}
    virtual PyObject* not_equal(PyObject *rhs){return 0;}
    virtual PyObject* ge(PyObject *rhs)       {return 0;}
    virtual PyObject* le(PyObject *rhs)       {return 0;}
};

#endif
