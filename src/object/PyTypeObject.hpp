//
// Created by mzygd on 2019/11/30.
//

#ifndef PY_TYPE_OBJECT_HPP_
#define PY_TYPE_OBJECT_HPP_

#include <mutex>

#include "Klass.hpp"
#include "PyObject.hpp"

class TypeKlass : public Klass {
public:
    static TypeKlass *get_instance();
    void print(PyObject *obj) override;
private:
    TypeKlass();
    static TypeKlass *m_Instance;
    static std::mutex m_Mutex;
};

class PyTypeObject : public PyObject {
friend class TypeKlass;
public:
    PyTypeObject();

    void set_own_klass(Klass* klass);
    Klass* own_klass();
private:
    Klass *m_OwnKlass;
};

#endif //PY_TYPE_OBJECT_HPP_
