#include "PyFunction.hpp"

#include <stdio.h>
#include <assert.h>

FunctionKlass* FunctionKlass::m_Instance = nullptr;
std::mutex FunctionKlass::m_Mutex;

FunctionKlass::FunctionKlass(){}

FunctionKlass* FunctionKlass::get_instance()
{
    if(m_Instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(m_Instance == nullptr)
        {
            m_Instance = new FunctionKlass();
        }
    }
    return m_Instance;
}

void FunctionKlass::print(PyObject *x)
{
    printf("<function: ");
    PyFunction *fun = dynamic_cast<PyFunction*>(x);
    assert(fun && fun->klass() == static_cast<Klass*>(this));
    //
    printf(">");
}


// PyFunction
PyFunction::PyFunction(CodeObject *codeObj);
{
    m_FuncCode = codeObj;
    m_FuncName = codeObj->m_CoName;
    m_Flags    = codeObj->m_Flag;
    set_klass(FunctionKlass::get_instance());
}

PyFunction::PyFunction(Klass *klass)
{
    m_FuncCode = nullptr;
    m_FuncName = nullptr;
    m_Flags    = 0;
    set_klass(klass);
}
