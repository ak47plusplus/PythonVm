#ifndef PYCELL_HPP_
#define PYCELL_HPP_

#include "Klass.hpp"
#include "PyTuple.hpp"
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
    PyTuple *table_; /* Not hold the ownership */
    int     index_; 
public:
    PyCell(PyTuple *table, int index);
    PyObject *get();
};

#endif 