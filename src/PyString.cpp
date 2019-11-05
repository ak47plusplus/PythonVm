#include "PyString.hpp"
#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include "VM.hpp"

#include <string.h>
#include <stdlib.h>
#include <assert.h>


StringKlass::StringKlass(){}

StringKlass *StringKlass::m_Instance = nullptr;
std::mutex StringKlass::m_Mutex;

StringKlass *StringKlass::get_instance()
{
      if(StringKlass::m_Instance == nullptr)
      {
            std::lock_guard<std::mutex> lock(StringKlass::m_Mutex);
            if(StringKlass::m_Instance == nullptr)
            {
                  StringKlass::m_Instance = new StringKlass();
            }
      }
      return StringKlass::m_Instance;
}

// py的字符串里可以包含\0 请勿使用%s打印
void StringKlass::print(PyObject *x)
{
    assert(x && x->klass() == static_cast<Klass*>(this));
    PyString *pyStr = dynamic_cast<PyString*>(x);
    for(int i = 0; i < pyStr->length(); i++)
    {
        printf("%c", pyStr->value()[i]);
    }
}

// python的str只能和str加
PyObject* StringKlass::add(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// TODO 字符串不能乘以字符串 但是能乘以int 自动翻倍字符串...
PyObject* StringKlass::mul(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// greater 只支持str和str比较
PyObject* StringKlass::greater(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// less 只支持str和str比较
PyObject* StringKlass::less(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// equal 支持str和任意类型比较
PyObject* StringKlass::equal(PyObject *lhs, PyObject *rhs)
{
    if(lhs->klass() != rhs->klass()) return VM::PyFalse;
    assert(lhs->klass() == static_cast<Klass*>(this));
    assert(rhs->klass() == static_cast<Klass*>(this));

    PyString *pLhs = dynamic_cast<PyString*>(lhs);
    PyString *pRhs = dynamic_cast<PyString*>(rhs);

    if(pLhs->length() != pRhs->length())
    {
        return VM::PyFalse;
    }

    for(int i = 0; i < pLhs->length(); ++i)
    {
        if(pLhs->value()[i] != pRhs->value()[i])
            return VM::PyFalse;
    }
    return VM::PyTrue;
}

// not equal 支持str和任意类型比较
PyObject* StringKlass::not_equal(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// ge 只支持str和str比较
PyObject* StringKlass::ge(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}

// le 只支持str和str比较
PyObject* StringKlass::le(PyObject *lhs, PyObject *rhs)
{
    return nullptr;
}



// =================================== stringobject=============================

// do not contain the '\0'
PyString::PyString(const char * str)
{
    m_length = strlen(str);
    m_value = new char[m_length];
    strcpy(m_value, str);
    set_klass(StringKlass::get_instance());
}

// here the '\0' is a valid char use memcpy instead of strcpy
PyString::PyString(const char * str, const int length)
{
    m_length = length;
    m_value = new char[length];
    memcpy(m_value, str, (size_t) m_length);
    set_klass(StringKlass::get_instance());
}

PyString::PyString(const PyString& rhs)
{
    delete[] m_value;
    m_length = rhs.m_length;
    m_value = new char[m_length];
    memcpy(this->m_value,rhs.m_value, (size_t)m_length);
    set_klass(StringKlass::get_instance());
}

PyString& PyString::operator=(const PyString &rhs)
{
    delete[] m_value;
    m_length = rhs.m_length;
    m_value = new char[m_length];
    memcpy(this->m_value,rhs.m_value, (size_t)m_length);
    return *this;
}

PyString::~PyString()
{
    if (m_value != nullptr) {
        delete[] m_value;
    }
}

PyString* PyString::times(const PyString* rawStr, int times)
{
    if (times <= 0) {
        return new PyString("", 0); // 空字符串
    }
    int oldLen = rawStr->length();
    int newLen = oldLen * times;
    char tmp[newLen];
    memcpy(tmp, rawStr->value(), oldLen);
    memcpy(tmp + oldLen, rawStr->value(), oldLen);
    memcpy(tmp + oldLen + oldLen, rawStr->value(), oldLen);
    return new PyString(tmp, newLen);
}
