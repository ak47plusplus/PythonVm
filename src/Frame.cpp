#include "Frame.hpp"

Frame::Frame(CodeObject *codes)
{
    m_Locals    = new Map<PyObject*,PyObject*>();
    m_Stack     = new ArrayList<PyObject*>(codes->m_StackSize);
    m_LoopStack = new ArrayList<Block*>();
    m_Consts    = codes->m_Consts;
    m_Names     = codes->m_Names;
    m_Codes     = codes;
    m_Pc        = 0;
}
Frame::Frame(){}

Frame::~Frame()
{
    delete m_Locals;
    delete m_Stack;
    delete m_LoopStack;
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
    return highBit << 8 | lowBit; // opArg只占2个字节的.
}
