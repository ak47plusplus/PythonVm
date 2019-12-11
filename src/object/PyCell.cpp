#include "PyCell.hpp"
#include "PyDict.hpp"

CellKlass * CellKlass::m_Instance = nullptr;
std::mutex  CellKlass::m_Mutex;

CellKlass::CellKlass()
{
    set_name("PyCell");
    set_attrs(new PyDict());
}

CellKlass* CellKlass::get_instance()
{
    if(m_Instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(m_Instance == nullptr)
        {
            m_Instance = new CellKlass();
        }
    }
    return m_Instance;
}

PyCell::PyCell(PyList *table, int index)
    : table_(table), index_(index)
{
    set_klass(CellKlass::get_instance());
}

PyObject *PyCell::get()
{
    return table_->get(index_);
}
