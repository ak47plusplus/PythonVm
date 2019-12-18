#ifndef PY_BOOL_HPP
#define PY_BOOL_HPP

#include "Klass.hpp"
#include "PyObject.hpp"

#include <memory>
#include <mutex>

class BoolKlass : public Klass {
public:
    static BoolKlass * get_instance();
    void InitKlass() override;
    void print(PyObject *x) override;
private:
    BoolKlass();
    static std::unique_ptr<BoolKlass> m_Instance;
    static std::mutex m_Mutex;
};

class PyBool : public PyObject {
public:
    PyBool();
    explicit PyBool(bool v);
    bool value() { return m_Value; }
private:
    bool m_Value;
};

#endif