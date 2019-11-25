#include "PyDictIterator.hpp"
#include "PyDict.hpp"

MAKE_CLASS_IMPL(DictKeyIteratorKlass, dict_keyiterator)

PyDictKeyIterator::PyDictKeyIterator(PyDict *target)
    : m_Target(target), m_Cursor(0)
{
    set_klass(DictKeyIteratorKlass::get_instance());
}

bool PyDictKeyIterator::has_next()
{
    return m_Cursor < m_Target->size();
}

PyObject* PyDictKeyIterator::next()
{
    auto cur = m_Cursor;
    if(cur >= m_Target->size())
    {
        __throw_python_except("NoSuchElementException\n");
    }
    m_Cursor = cur + 1;
    return m_Target->get_key(cur);
}