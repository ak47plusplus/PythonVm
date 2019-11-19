#ifndef KLASS_HPP___
#define KLASS_HPP___

#include <map>
#include <string>
#include "Panic.hpp"

// Forward declaration
class PyString;
class PyObject;
class PyList;
class PyDict;

class Klass {
public:
    Klass();
    ~Klass();
    void set_name(const char *name)                           {m_Name.assign(name);}
    std::string& name()                                       {return m_Name;}


    virtual void print(PyObject *x)                          {}

    virtual PyObject* add(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* sub(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* div(PyObject *lhs, PyObject *rhs)      {return 0;}
    virtual PyObject* mod(PyObject *lhs, PyObject *rhs)      {return 0;}


    virtual PyObject* greater(PyObject *lhs, PyObject *rhs)  {return 0;}
    virtual PyObject* less(PyObject *lhs, PyObject *rhs)     {return 0;}
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs)    {return 0;}
    virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs){return 0;}
    virtual PyObject* ge(PyObject *lhs, PyObject *rhs)       {return 0;}
    virtual PyObject* le(PyObject *lhs, PyObject *rhs)       {return 0;}

    virtual PyObject* len(PyObject *x)
    {
        __throw_python_except("TypeError: object oif type '%s' has no len()\n", m_Name.c_str());
        return 0;
    }
    virtual PyObject* iter(PyObject *self)
    {
        __throw_python_except("TypeError:%s object is not iterable.\n", m_Name.c_str());
        return 0;
    }
    virtual PyObject* subscr(PyObject *lhs, PyObject *rhs)   
    {
        __throw_python_except("TypeError:%s object is not subscriptable.\n", m_Name.c_str());
        return 0;
    }
    virtual PyObject* store_subscr(PyObject *lhs, PyObject *mhs, PyObject *rhs) 
    {
        __throw_python_except("TypeError:%s object doesn't support item assignment.\n", m_Name.c_str());
        return 0;
    }
    virtual PyObject* del_subscr(PyObject *lhs, PyObject *rhs) 
    {
        __throw_python_except("TypeError:%s object doesn't support item deletion.\n", m_Name.c_str());
        return 0;
    }

    virtual PyObject* contains(PyObject *lhs, PyObject *rhs) {return 0;} // 1 in lst

    void register_klass_dict(PyObject* k, PyObject* v);
    std::map<PyObject*, PyObject*> klass_dict()              {return m_KlassDict; }
    virtual PyObject* getattr(PyObject *lhs, PyObject *rhs);

private:
    std::string                      m_Name;
    std::map<PyObject*, PyObject*>   m_KlassDict;
};

#endif
