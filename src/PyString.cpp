#include "PyString.hpp"
#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include "Panic.hpp"
#include "VM.hpp"

#include <string>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


StringKlass::StringKlass(){
    set_name("str");
}

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
    printf("'");
    for(int i = 0; i < pyStr->length(); i++)
    {
        printf("%c", pyStr->value()[i]);
    }
     printf("'");
}

// python的str只能和str加
PyObject* StringKlass::add(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == this);
    if(rhs == nullptr || rhs->klass() != StringKlass::get_instance())
    {
        __panic("TypeError: must be str, not int\n");
    }
    PyString *str1 = dynamic_cast<PyString*>(lhs);
    PyString *str2 = dynamic_cast<PyString*>(rhs);
    auto str1_len = str1->length();
    auto str2_len = str2->length();
    char *buffer = new char[str1_len + str2_len];
    memcpy(buffer, str1->value(), str1_len);
    memcpy(buffer + str1_len, str2->value(), str2_len);
    PyString *res = new PyString(buffer, str1_len + str2_len);
    delete[] buffer;
    return res;
}

// TODO 字符串不能乘以字符串 但是能乘以int 自动翻倍字符串...
PyObject* StringKlass::mul(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == this);
    if(rhs == nullptr || rhs->klass() != IntegerKlass::get_instance())
    {
        __panic("TypeError: can't multiply sequence by non-int of type 'str'");
    }
    // TODO
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

PyObject* StringKlass::len(PyObject *x)
{
    return new PyInteger(dynamic_cast<PyString*>(x)->length());
}

PyObject* StringKlass::subscr(PyObject *lhs, PyObject* rhs)
{
    assert(lhs && lhs->klass() == this);
    if(rhs == nullptr || rhs->klass() != IntegerKlass::get_instance())
    {
        __panic("TypeError: string indices must be inntegers\n");
    }
    PyString *pyStr = dynamic_cast<PyString*>(lhs);
    PyInteger *index = dynamic_cast<PyInteger*>(rhs);
    char  target = pyStr->value()[index->value()];
    return new PyString(&target, 1);
}

PyObject* StringKlass::contains(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == this);
    if(rhs == nullptr || rhs->klass() != StringKlass::get_instance())
    {
        __panic("TypeError: 'in <string>' requires string as left operand, not ?\n");
    }
    PyString *base_str = dynamic_cast<PyString*>(lhs);
    PyString *sub_str = dynamic_cast<PyString*>(rhs);
    if(sub_str->length() > base_str->length())
    {
        return VM::PyFalse;
    }
    // 避免使用strstr,因为PyString可以包含'\0' 这里直接使用std::string来实现.
    std::string _base(base_str->value(), base_str->length());
    std::string _sub(sub_str->value(), sub_str->length());
    return _base.find(_sub) == std::string::npos ? VM::PyFalse : VM::PyTrue;
}


// =================================== stringobject=============================

// do not contain the '\0'
PyString::PyString(const char * str)
{
    if(str == nullptr)
    {
        m_Length = 0;
        m_Value = nullptr;
    } else
    {
        m_Length = strlen(str);
        m_Value = new char[m_Length];
        strcpy(m_Value, str);
    }
    set_klass(StringKlass::get_instance());
}

// here the '\0' is a valid char use memcpy instead of strcpy
PyString::PyString(const char * str, const int length)
{
    m_Length = length;
    m_Value = new char[length];
    memcpy(m_Value, str, (size_t) m_Length);
    set_klass(StringKlass::get_instance());
}

PyString::PyString(const PyString& rhs)
{
    delete[] m_Value;
    m_Length = rhs.m_Length;
    m_Value = new char[m_Length];
    memcpy(this->m_Value,rhs.m_Value, (size_t)m_Length);
    set_klass(StringKlass::get_instance());
}

PyString& PyString::operator=(const PyString &rhs)
{
    delete[] m_Value;
    m_Length = rhs.m_Length;
    m_Value = new char[m_Length];
    memcpy(this->m_Value,rhs.m_Value, (size_t)m_Length);
    return *this;
}

PyString::~PyString()
{
    if (m_Value != nullptr) {
        delete[] m_Value;
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

PyString* PyString::empty_str()
{
    return new PyString(nullptr);
}


namespace pystring {
    PyObject* string_upper(FuncArgs args)
    {
        auto arg0 = args->get(0);
        assert(arg0->klass() == StringKlass::get_instance());
        PyString *str = dynamic_cast<PyString*>(arg0);
        int len = str->length();
        if(len <= 0)
            return VM::PyNone;
        char upper[len];
        char c;
        for(auto i = 0; i < len; i++)
        {
            c = str->value()[i];
            if(c >= 'a' && c <= 'z')
                upper[i] = c - 32;
            else
                upper[i] = c;
        }
        return new PyString(upper,len);
    }
}
