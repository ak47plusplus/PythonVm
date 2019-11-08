#ifndef PY_FUNCTION_HPP__
#define PY_FUNCTION_HPP__

#include "Core.hpp"
#include "Klass.hpp"
#include "Frame.hpp"
#include "CodeObject.hpp"

#include <mutex>

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
public:
    PyFunction(CodeObject *codeObj);
    PyFunction(Klass *klass);
    PyString* func_name()   { return m_FuncName;}
    uint32_t  flags()       { return m_Flags;}
private:
    CodeObject *m_FuncCode;
    PyString   *m_FuncName;
    uint32_t    m_Flags;
};

#endif
