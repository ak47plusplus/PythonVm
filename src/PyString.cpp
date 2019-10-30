#include "PyString.hpp"
#include <cstring>
#include <cstdlib>

// do not contain the '\0'
PyString::PyString(const char * str)
{
    m_length = strlen(str);
    m_value = new char[m_length];
    strcpy(m_value, str);

}

// here the '\0' is a valid char use memcpy instead of strcpy
PyString::PyString(const char * str, const int length)
{
    m_length = length;
    m_value = new char[length];
    memcpy(m_value, str);
}

PyString::PyString(const PyString& rhs)
{
    delete[] m_value;
    m_length = rhs.m_length;
    m_value = new char[m_length];
    memcpy(this->m_value,rhs.m_value);
}
PyString& PyString::operator=(const PyString &rhs)
{
  delete[] m_value;
  m_length = rhs.m_length;
  m_value = new char[m_length];
  memcpy(this->m_value,rhs.m_value);
  return *this;
}
