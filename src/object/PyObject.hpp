#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

#define PyObject_NotNullPtr(objPtr) ((objPtr) != nullptr)
#define PyObject_Klass_Check(objPtr, klsPtr)  (((objPtr) != nullptr) && ((objPtr)->klass()) == klsPtr)
#define PyObject_Klass_Check0(objPtr, kls)  (((objPtr) != nullptr) && ((objPtr)->klass()) == kls::get_instance())

class Klass;

class PyObject {
public:
    virtual ~PyObject() = default;

    Klass* klass()              { return m_Klass;}
    void set_klass(Klass * kls) { m_Klass = kls;}

    void print();

    PyObject *id();

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

    PyObject *len();
    PyObject *iter();
    PyObject *subscr(PyObject *rhs);
    PyObject *store_subscr(PyObject *mhs, PyObject *rhs);
    PyObject *del_subscr(PyObject *rhs);

    virtual PyObject *contains(PyObject *rhs);

    PyObject *getattr(PyObject *attr_name);

private:
    Klass *m_Klass;
};

#endif
