#ifndef PY_FUNCTION_HPP__
#define PY_FUNCTION_HPP__

#include "Core.hpp"
#include "Map.hpp"
#include "Klass.hpp"
#include "Frame.hpp"
#include "CodeObject.hpp"

#include <mutex>

// 指向native函数的函数指针
typedef PyObject* (*NativeFunctionPtr)(ArrayList<PyObject*> *args);

class NativeFunctionKlass : public Klass {
public:
    static NativeFunctionKlass* get_instance();
private:
    static NativeFunctionKlass* m_Instance;
    static std::mutex           m_Mutex;
    NativeFunctionKlass();
};

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
    CodeObject                  *m_FuncCode  __PTR_IN__;
    PyString                    *m_FuncName  __PTR_IN__;
    uint32_t                     m_Flags;
    ArrayList<PyObject*>        *m_DefaultArgs __PTR_INNER__;
    Map<PyObject*, PyObject*>   *m_Globals __PTR_IN__; // 函数所依赖的全局变量表是定义函数对象的时候的，而不是调用函数时候的 这个字段纯属无奈之举
    NativeFunctionPtr            m_NativeFunctionPtr;
public:
    PyFunction(CodeObject *codeObj);
    PyFunction(Klass *klass);
    ~PyFunction();
    PyString*                   func_name()   { return m_FuncName;}
    uint32_t                    flags()       { return m_Flags;}
    Map<PyObject*, PyObject*>  *globals()     { return m_Globals;}
    void                        set_globals(Map<PyObject*, PyObject*>  *_globals)
                                              { m_Globals = _globals;}
    ArrayList<PyObject*>       *default_args(){ return m_DefaultArgs;}
    void                        set_default_args(ArrayList<PyObject*> *_default_args);

    // just for native
    PyFunction(NativeFunctionPtr nativeFuncPtr);
    PyObject*                   native_call(ArrayList<PyObject*> *args);
};

#endif
