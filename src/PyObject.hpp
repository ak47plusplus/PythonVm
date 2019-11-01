#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

class PyObject {
public:
    virtual void print() {}
    virtual PyObject* add(PyObject *rhs) {}
};

#endif
