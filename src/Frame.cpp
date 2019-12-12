#include "Frame.hpp"
#include "VM.hpp"
#include "Core.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "PyString.hpp"
#include "PyFunction.hpp"

#include <cassert>

// This method is used by module only.
Frame::Frame(CodeObject *codes)
{
    m_Stack     = new PyList();

    m_LoopStack = new ArrayList<Block*>();
    m_Codes     = codes;
    m_Consts    = codes->m_Consts;
    m_Names     = codes->m_Names;
    
    m_Locals    = new PyDict();
    m_Globals   = m_Locals;     /* In non function context, global and local are equivalent */
    m_FastLocals= nullptr;
    m_Closure   = nullptr;
    
    m_Pc        = 0;
    m_Caller    = nullptr;
}

/**
 * Create a Frame by a function object.
 * @param func The function object.
 * @param args The arguments for the function.
 * @param opArg The opArg for opCode named CALL_FUNCTION.
 */
Frame::Frame(PyFunction *func, ArrayList<PyObject*> *args, int opArg)
{
    assert((args != nullptr && opArg != 0) || (args == nullptr && opArg == 0));

    m_Stack     = new PyList();
    m_LoopStack = new ArrayList<Block*>();
    m_Codes     = func->m_FuncCode;
    m_Consts    = m_Codes->m_Consts;
    m_Names     = m_Codes->m_Names;

    m_Locals    = new PyDict(8);
    m_Globals   = func->globals();
    m_FastLocals= new PyList();

    m_Pc        = 0;
    m_Caller    = nullptr;

    const int argcnt = m_Codes->m_ArgCount;
    const int l8b = opArg & 0xff;
    const int h8b = opArg & 0xff00;
    int kwPos = argcnt;

    /* Padding the default arguments. */
    if(func->default_args())
    {
        int defaultArgCnt = func->default_args()->size();
        int neededArgCnt    = m_Codes->m_ArgCount;
        while (defaultArgCnt--)
        {
            m_FastLocals->set(-- neededArgCnt, func->default_args()->get(defaultArgCnt));
        }
    }
    PyList *list = nullptr; /* For Variable parameters */
    PyDict *dict = nullptr; /* For K-V parameters */

    if(argcnt < l8b)
    {
        int i = 0;
        for(; i < argcnt; i++)
        {
            m_FastLocals->set(i, args->get(i));
        }
        list = new PyList();
        for(; i < l8b; i++)
        {
            list->push(args->get(i));
        }
    }
    else 
    {
        for(int i = 0; i < l8b; i++)
            m_FastLocals->set(i, args->get(i));
    }

    for(int i = 0; i < h8b; i++)
    {
        /* Don't worry, this is already constrained by the compiler */
        PyObject *k = args->get(l8b + 2 * i);
        PyObject *v = args->get(l8b + 2 * i + 1);

        /* If the key matches the element in varnames, the key is not an 
         * extension key argument, else it was a extension key arguments.
         */
        int idx = m_Codes->m_Varnames->index(k);
        if(idx >= 0)
        {
            m_FastLocals->set(idx, v);
        }
        else {
            if(dict == nullptr) dict = new PyDict();
            dict->put(k, v);
        }
    }

    /* Check and Padding VARARGS */
    if(m_Codes->m_Flag & PyFunction::CO_VARARGS)
    {
        if(list == nullptr) list = new PyList();
        m_FastLocals->set(argcnt, list);
        kwPos++;
    }
    else {
        if(list != nullptr)
            __throw_python_except("TypeError: too many parameters was given.\n");
    }

    /* Check and Padding KEYWORDARGS */
    if(m_Codes->m_Flag & PyFunction::CO_VARKEYWORDS)
    {
        if(dict == nullptr) dict = new PyDict();
        m_FastLocals->set(kwPos, dict);
    }
    else {
         if(dict != nullptr)
            __throw_python_except("TypeError: too many keyword parameter was given.\n");
    }

    // 处理闭包 多层闭包时需要调整闭包变量的顺序
    m_Closure = func->closure();
    if(m_Closure == nullptr)
        m_Closure = new PyTuple();

}

Frame::Frame(){}

Frame::~Frame()
{
    delete m_Stack;
    delete m_LoopStack;
    delete m_Locals;
    // delete m_Globals; need delete ?
    m_Locals = nullptr;
    m_Stack = nullptr;
    m_LoopStack = nullptr;
    m_Consts = nullptr;
    m_Names = nullptr;
    m_Codes = nullptr; // ? code is need to delete ?
    m_Pc = 0;
}

bool Frame::has_more_codes()
{
    return m_Pc < m_Codes->m_ByteCodes->length();
}

uint8_t Frame::get_op_code()
{
    return m_Codes->m_ByteCodes->value()[m_Pc++];
}

uint32_t Frame::get_op_arg()
{
    int lowBit =  m_Codes->m_ByteCodes->value()[m_Pc++] & 0xff;
    int highBit = m_Codes->m_ByteCodes->value()[m_Pc++] & 0xff;
    return highBit << 8 | lowBit;
}

/**
 * For example:
 * <pre>
 * def test(x):
 *     def say():
 *          print x
 *      return say
 * f = test(3)
 * f()
 * </pre>
 * the x is not the local name, it was a cell var but stored in the func args
 * @param idx
 * @return
 */
PyObject *Frame::get_cell_from_func_arguments(int idx)
{
    PyObject * cellName = m_Codes->m_Cellvars->get(idx);
    idx = m_Codes->m_Varnames->index(cellName);
    assert(idx != -1);
    return m_FastLocals->get(idx);
}