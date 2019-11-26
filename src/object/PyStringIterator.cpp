#include "PyStringIterator.hpp"
#include "Klass.hpp"
#include "PyString.hpp"

MAKE_CLASS_IMPL(StringIteratorKlass, stringiterator)


PyStringIterator::PyStringIterator(PyString *pyStr)
    :m_Target(pyStr), m_Cursor(0)
{

}

bool PyStringIterator::has_next()
{
    return m_Cursor < m_Target->length(); // ok for eq.
}

PyObject* PyStringIterator::next()
{
    auto cur = m_Cursor;
    if(cur >= m_Target->length())
    {
        __throw_python_except("NoSuchElementException\n");
    }
    m_Cursor = cur + 1;
    return new PyString((char *)(m_Target->value() + cur), 1);
}