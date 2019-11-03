#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

class Klass;

class PyObject {
public:
    virtual ~PyObject() {}

    Klass* klass()              { return m_Klass;}
    void set_klass(Klass * kls) { m_Klass = kls;}

    void print();

    PyObject *add(PyObject *rhs);
    PyObject *sub(PyObject *rhs);
    PyObject *mul(PyObject *rhs);
    PyObject *div(PyObject *rhs);
    PyObject *mod(PyObject *rhs);

    PyObject *greater(PyObject *rhs);
    PyObject *less(PyObject *rhs);
    PyObject *equal(PyObject *rhs);
    PyObject *not_equal(PyObject *rhs);
    PyObject *ge(PyObject *rhs);
    PyObject *le(PyObject *rhs);

private:
    Klass *m_Klass;
};

#endif
