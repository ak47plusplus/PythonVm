#ifndef PYCELL_HPP_
#define PYCELL_HPP_

#include "Klass.hpp"
#include "PyList.hpp"
#include "PyObject.hpp"

#include <mutex>

class CellKlass : public Klass {
public:
    static CellKlass* get_instance();
private:
    CellKlass();
    static CellKlass *m_Instance;
    static std::mutex m_Mutex;
};

class PyCell : public PyObject {
friend class CellKlass;
private:
    PyList *table_; /* Not hold the ownership */
    int     index_; 
public:
    PyCell(PyList *table, int index);
    PyObject *get();
};

#endif 