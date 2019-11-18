#include "PyListIterator.hpp"
#include "PyList.hpp"
#include "Panic.hpp"

ListIteratorKlass* ListIteratorKlass::m_Instance = nullptr;
std::mutex ListIteratorKlass::m_Mutex;

ListIteratorKlass::ListIteratorKlass()
{
    set_name("listiterator");
}

ListIteratorKlass *ListIteratorKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new ListIteratorKlass();
        }
    }
    return m_Instance;
}

PyListIterator::PyListIterator(PyList *list)
    : m_Target(list), m_Cursor(0)
{

}

bool PyListIterator::has_next()
{
    return m_Cursor < m_Target->size(); // ok for eq.
}

PyObject *PyListIterator::next()
{
    auto cur = m_Cursor;
    if(cur >= m_Target->size())
    {
        __throw_python_except("NoSuchElementException\n");
    }
    m_Cursor = cur + 1;
    return m_Target->get(cur);
}