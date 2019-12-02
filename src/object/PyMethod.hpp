#ifndef PY_METHOD_HPP
#define PY_METHOD_HPP

#include "Klass.hpp"
#include "PyObject.hpp"
#include "PyFunction.hpp"

#include <mutex>

class MethodKlass : public Klass {
public:
    MethodKlass *get_instance();
private:
    MethodKlass();
    static MethodKlass *m_Instance;
    static std::mutex   m_Mutex;
};


/**
 * obj = Obj()
 * obj.func(1,2,3) == func(obj, 1, 2,3) the first arg is the obj self.
 */
class PyMethod : public PyObject {
public:
    PyMethod(PyFunction *func);
    PyMethod(PyObject *owner, PyFunction *func);

    void        set_owner(PyObject *owner) { m_Owner = owner; }
    PyObject   *owner()                    { return m_Owner; }
    PyFunction *func()                     { return m_Func;}
public:
    static bool is_function(PyObject *obj);
    static bool is_yield_function(PyObject *obj);

private:
    PyObject   *m_Owner;
    PyFunction *m_Func;
};

#endif
