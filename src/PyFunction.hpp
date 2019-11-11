#ifndef PY_FUNCTION_HPP__
#define PY_FUNCTION_HPP__

#include "Core.hpp"
#include "Map.hpp"
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

private:
    CodeObject *m_FuncCode;
    PyString   *m_FuncName;
    uint32_t    m_Flags;

    Map<PyObject*, PyObject*> *m_Globals;
public:
    PyFunction(CodeObject *codeObj);
    PyFunction(Klass *klass);
    PyString*                   func_name()   { return m_FuncName;}
    uint32_t                    flags()       { return m_Flags;}
    Map<PyObject*, PyObject*>  *globals()     { return m_Globals;}
    void                        set_globals(Map<PyObject*, PyObject*>  *_globals)
                                              { m_Globals = _globals;}
};

#endif
