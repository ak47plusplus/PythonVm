#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

class PyObject {
public:
    virtual void print() = 0;
    virtual PyObject* add(PyObject *rhs) = 0;
};

#endif
