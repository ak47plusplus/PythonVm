#ifndef PY_FUNCTION_HPP__
#define PY_FUNCTION_HPP__

#include "Core.hpp"
#include "Map.hpp"
#include "Klass.hpp"
#include "Frame.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"
#include "PyDict.hpp"
#include "PyList.hpp"
#include "PyTuple.hpp"

#include <mutex>

 class PyFunction;

typedef PyObject* (*NativeFunctionPtr)(ArrayList<PyObject*> *args);


class NativeFunctionKlass : public Klass {
public:
    static NativeFunctionKlass* get_instance();
private:
    static NativeFunctionKlass* m_Instance;
    static std::mutex           m_Mutex;
    NativeFunctionKlass();
};

/**
 * MethodKlass is use of the function defined in the class.
 */
 class MethodKlass : public Klass {
 public:
     static MethodKlass *get_instance();
 private:
     MethodKlass();
     static MethodKlass *m_Instance;
     static std::mutex   m_Mutex;
 };

 class PyMethod : public PyObject {
 public:
     PyMethod(PyFunction *func);
     PyMethod(PyObject *owner, PyFunction *func);

     void        set_owner(PyObject *owner) { m_Owner = owner; }
     PyObject   *owner()                    { return m_Owner; }
     PyFunction *func()                     { return m_Func;}
 public:
     static bool is_function(PyObject *obj);
     static bool is_yield_function(PyObject *obj);

 private:
     PyObject   *m_Owner;
     PyFunction *m_Func;
 };

/**
 * FunctionKlass is a klass of the functions.
 */
class FunctionKlass : public Klass {
public:
    static FunctionKlass* get_instance();
    virtual void print(PyObject *x);
private:
    FunctionKlass();
    static FunctionKlass* m_Instance;
    static std::mutex     m_Mutex;
};

class PyFunction : public PyObject {
friend class Frame;
friend class FunctionKlass;
private:
    CodeObject                  *m_FuncCode;
    PyString                    *m_FuncName;
    uint32_t                     m_Flags;
    ArrayList<PyObject*>        *m_DefaultArgs;
    PyDict                      *m_Globals; // 函数所依赖的全局变量表是定义函数对象的时候的，而不是调用函数时候的
    PyTuple                     *m_Closure; // 函数所捕获的闭包变量是定义函数对象的时候的，而不是调用函数时候的
    NativeFunctionPtr            m_NativeFunctionPtr;
public:
    PyFunction(CodeObject *codeObj);
    PyFunction(Klass *klass);
    ~PyFunction();
    PyString*                   func_name()                 { return m_FuncName;}
    uint32_t                    flags()                     { return m_Flags;}
    PyDict                     *globals()                   { return m_Globals;}
    void                        set_globals(PyDict* gl)     { m_Globals = gl;}
    ArrayList<PyObject*>       *default_args()              { return m_DefaultArgs;}
    void                        set_default_args(ArrayList<PyObject*> *_default_args);
    PyTuple                    *closure()                   {return m_Closure; }
    void                        set_closure(PyTuple *cls)   { m_Closure = cls; }
    /* Just Use for the native function */
    PyFunction(NativeFunctionPtr nativeFuncPtr);
    PyObject*                   native_call(ArrayList<PyObject*> *args);
public:
    enum CO_FLAGS {
        CO_VARARGS = 0x4,
        CO_VARKEYWORDS = 0x8,
        CO_GENERATOR = 0x20,
    };
};

#endif
