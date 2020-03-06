#ifndef KLASS_HPP___
#define KLASS_HPP___

#include <map>
#include <string>
#include "Panic.hpp"

#define MAKE_KLASS(_kls_)                               \
    class _kls_ : public Klass {                        \
    public:                                             \
        static _kls_ *get_instance();                   \
    private:                                            \
        _kls_();                                        \
        static _kls_ *m_Instance;                       \
        static std::mutex m_Mutex;                      \
    };

#define MAKE_CLASS_IMPL(_kls_, _kls_name_)              \
    _kls_ * _kls_::m_Instance = nullptr;                \
    std::mutex _kls_::m_Mutex;                          \
    _kls_::_kls_(){                                     \
    set_name(#_kls_name_);                              \
    }                                                   \
    _kls_ * _kls_::get_instance()                       \
    {                                                   \
        if(nullptr == m_Instance)                       \
        {                                               \
            std::lock_guard<std::mutex> lock(m_Mutex);  \
            if(nullptr == m_Instance)                   \
            {                                           \
                m_Instance = new _kls_();               \
            }                                           \
        }                                               \
        return m_Instance;                              \
    }

// Forward declaration
class PyString;
class PyObject;
class PyList;
class PyDict;
class PyTypeObject;
template<typename T>
class ArrayList;

class Klass {
public:
    using FuncArgs = ArrayList<PyObject*> *;
public:
    Klass() = default;
    ~Klass() = default;

    void set_name(const char *name)  { m_Name.assign(name);}
    std::string& name()              { return m_Name;      }

    void set_attrs(PyDict *attrs_)   { m_Attrs = attrs_;   }
    PyDict *attrs()                  { return m_Attrs;     }

    PyTypeObject* type_object()             { return m_TypeObject; }
    void set_type_object(PyTypeObject *obj) { m_TypeObject = obj; }

    void set_super(Klass *super_)   { m_Super = super_; }
    Klass* supper()                 { return m_Super; }

    virtual void InitKlass()              {;}


    /* virtuals */

    virtual void print(PyObject *x)                          {;}
    virtual PyObject* toBool(PyObject *x);

	// for the Custom Class we need impl it instead of impl only by the child-class.
    virtual PyObject* allocate_instance(PyObject *typeObj, Klass::FuncArgs args);

    virtual PyObject* add(PyObject *lhs, PyObject *rhs)      {return nullptr;}
    virtual PyObject* sub(PyObject *lhs, PyObject *rhs)      {return nullptr;}
    virtual PyObject* mul(PyObject *lhs, PyObject *rhs)      {return nullptr;}
    virtual PyObject* div(PyObject *lhs, PyObject *rhs)      {return nullptr;}d
    virtual PyObject* mod(PyObject *lhs, PyObject *rhs)      {return nullptr;}


    virtual PyObject* greater(PyObject *lhs, PyObject *rhs)  {return nullptr;}
    virtual PyObject* less(PyObject *lhs, PyObject *rhs)     {return nullptr;}
    virtual PyObject* equal(PyObject *lhs, PyObject *rhs)    {return nullptr;}
    virtual PyObject* not_equal(PyObject *lhs, PyObject *rhs){return nullptr;}
    virtual PyObject* ge(PyObject *lhs, PyObject *rhs)       {return nullptr;}
    virtual PyObject* le(PyObject *lhs, PyObject *rhs)       {return nullptr;}

    virtual PyObject* len(PyObject *x)
    {
        __throw_python_except("TypeError: object oif type '%s' has no len()\n", m_Name.c_str());
        return nullptr;
    }
    virtual PyObject* iter(PyObject *self)
    {
        __throw_python_except("TypeError:%s object is not iterable.\n", m_Name.c_str());
        return nullptr;
    }
    virtual PyObject* subscr(PyObject *lhs, PyObject *rhs)
    {
        __throw_python_except("TypeError:%s object is not subscriptable.\n", m_Name.c_str());
        return nullptr;
    }
    virtual PyObject* store_subscr(PyObject *lhs, PyObject *mhs, PyObject *rhs)
    {
        __throw_python_except("TypeError:%s object doesn't support item assignment.\n", m_Name.c_str());
        return nullptr;
    }
    virtual PyObject* del_subscr(PyObject *lhs, PyObject *rhs)
    {
        __throw_python_except("TypeError:%s object doesn't support item deletion.\n", m_Name.c_str());
        return nullptr;
    }

    virtual PyObject* contains(PyObject *lhs, PyObject *rhs) { return nullptr;} // 1 in lst
    // virtual PyObject* getattr(PyObject *lhs, PyObject *rhs);

public:
    static PyObject* create_klass(PyObject *locals, PyObject *supers, PyObject * className);
private:
    std::string     m_Name;  /* The name of the klass. */
    Klass           *m_Super{};
    PyTypeObject    *m_TypeObject{};
    PyDict          *m_Attrs{}; /* method and fields are attributes of the klass. */
};

#endif
