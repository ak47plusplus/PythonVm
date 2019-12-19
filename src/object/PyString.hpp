#ifndef PY_STRING_HPP
#define PY_STRING_HPP

#include <mutex>      // for std::mutex and std::lock_guard

#include "Klass.hpp"
#include "PyObject.hpp"
#include "ArrayList.hpp"

/**
 *  @brief The Python String class and object.
 *  @Created by GuoXiang 2019/10/29
 */

class StringKlass : public Klass
{
public:
    static StringKlass *get_instance();

    void InitKlass() override;

    void print(PyObject *x) override;
    PyObject *toBool(PyObject *x) override;

    // python的str只能和str加
    PyObject* add(PyObject *lhs, PyObject *rhs) override;

    // python的str不支持减法
    // virtual PyObject* sub(PyObject *lhs, PyObject *rhs);

    // TODO 字符串不能乘以字符串 但是能乘以int 自动翻倍字符串...
    PyObject* mul(PyObject *lhs, PyObject *rhs) override;

    // python的str不支持除法
    // virtual PyObject* div(PyObject *lhs, PyObject *rhs);

    // python的str不支持mod
    // virtual PyObject* mod(PyObject *lhs, PyObject *rhs);

    // greater 只支持str和str比较
    PyObject* greater(PyObject *lhs, PyObject *rhs) override;

    // less 只支持str和str比较
    PyObject* less(PyObject *lhs, PyObject *rhs) override;

    // equal 支持str和任意类型比较
    PyObject* equal(PyObject *lhs, PyObject *rhs) override;

    // not equal 支持str和任意类型比较
    PyObject* not_equal(PyObject *lhs, PyObject *rhs) override;

    // ge 只支持str和str比较
    PyObject* ge(PyObject *lhs, PyObject *rhs) override;

    // le 只支持str和str比较
    PyObject* le(PyObject *lhs, PyObject *rhs) override;

    PyObject* len(PyObject *x) override;
    PyObject* iter(PyObject *self) override;

    // 只支持取下标 不支持下标赋值和删除下标.
    PyObject* subscr(PyObject *lhs, PyObject* rhs) override;
    PyObject* contains(PyObject *lhs, PyObject *rhs) override;

private:
    StringKlass();
    static StringKlass *m_Instance;
    static std::mutex   m_Mutex;
};

class PyString : public PyObject {

public:
    explicit PyString(const char * str);
    PyString(const char * str, int length);
    PyString(const PyString& rhs);
    ~PyString() override;
    PyString& operator=(const PyString& rhs);
    const char *value() const   { return this->m_Value;}
    int length()        const   { return this->m_Length;}
public:
    static PyString* times(const PyString *rawStr, int times);
    static PyString* empty_str();
private:
    char * m_Value;
    int    m_Length;
};

namespace pystring {
    typedef ArrayList<PyObject*> *FuncArgs;
    extern PyObject* string_upper(FuncArgs args);
}


#endif
