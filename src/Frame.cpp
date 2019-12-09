#include "Frame.hpp"
#include "VM.hpp"
#include "Core.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "PyString.hpp"
#include "PyFunction.hpp"

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
    m_Stack     = new PyList();

    m_LoopStack = new ArrayList<Block*>();
    m_Codes     = func->m_FuncCode;
    m_Consts    = m_Codes->m_Consts;
    m_Names     = m_Codes->m_Names;

    m_Locals    = new PyDict(8);
    m_Globals   = func->globals();
    m_FastLocals= new PyList();
    m_Closure   = new PyList();

    m_Pc        = 0;
    m_Caller    = nullptr;
    
    if(func->default_args())
    {
        int defaultArgCnt = func->default_args()->size();
        int neededArgCnt    = m_Codes->m_ArgCount;
        while (defaultArgCnt--) // 默认参数只支持从后往前
        {
            m_FastLocals->set(-- neededArgCnt, func->default_args()->get(defaultArgCnt));
        }
    }
    if(args)
    {
        for(auto i = 0; i < args->size(); i++)
        {
            m_FastLocals->set(i, args->get(i));
        }
    }
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
