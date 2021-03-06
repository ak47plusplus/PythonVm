#ifndef PY_OBJECT_HPP
#define PY_OBJECT_HPP

#include "Klass.hpp"
#include <memory>
#include <mutex>

class ObjectKlass : public Klass {
public:
    static ObjectKlass* get_instance();
private:
    static std::unique_ptr<ObjectKlass> m_Instance;
    static std::mutex m_Mutex;
    ObjectKlass();
};

class PyObject {
public:
    virtual ~PyObject() = default;

    Klass* klass()              { return m_Klass;}
    void set_klass(Klass * kls) { m_Klass = kls;}

    void print();

    PyObject *id();
    PyObject* toBool();

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
