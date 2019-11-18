#ifndef PY_DICT_HPP__
#define PY_DICT_HPP__

#include "Klass.hpp"
#include "PyObject.hpp"

#include <mutex>

class DictKlass : public Klass {
public:
    static DictKlass* get_instance();
private:
    static DictKlass     *m_Instance;
    static std::mutex    m_Mutex;
    DictKlass();
};

class PyDict : public PyObject {
public:

private:
};

#endif
