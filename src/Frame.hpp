#ifndef FRAME_HPP__
#define FRAME_HPP__

#include <stdint.h>

#include "Map.hpp"
#include "Core.hpp"
#include "Block.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"

class PyFunction;

/**
 * Runtime Frame. 
 */
class Frame {
public:
    Frame(CodeObject *codes);
    Frame(PyFunction *func, ArrayList<PyObject*> *args, int opArg);
    Frame();
    ~Frame();
public:
    ArrayList<PyObject*> *     stack()             { return m_Stack;}
    ArrayList<Block*>*         loop_stack()        { return m_LoopStack;}
    ArrayList<PyObject*> *     consts()            { return m_Consts;}
    ArrayList<PyObject*> *     names()             { return m_Names;}
    Map<PyObject*, PyObject*>* locals()            { return m_Locals;}
    void                       set_pc(pc_t pc)     { m_Pc = pc;}
    pc_t                       get_pc()            { return m_Pc;}
    Frame                     *caller()            { return m_Caller;}
    void                       set_caller(Frame *f){ m_Caller = f;}
    bool                       is_first_frame()    { return m_Caller == nullptr;}
    bool                       has_more_codes();
    uint8_t                    get_op_code();
    uint32_t                   get_op_arg();
    Map<PyObject*, PyObject*>* globals()           { return m_Globals;}
    ArrayList<PyObject*>      *fastLocals()        { return m_FastLocals;}
public:
    ArrayList<PyObject*>        *m_Stack;       /* 运行时栈 */
    ArrayList<Block*>           *m_LoopStack;   /* 用于保存循环的Block栈 */
    ArrayList<PyObject*>        *m_Consts;      /* 常量表 函数定义也存在里面 */
    ArrayList<PyObject*>        *m_Names;       /* 符号表 */

    Map<PyObject*, PyObject*>   *m_Locals;      /* 当前栈帧中的局部变量 */
    Map<PyObject*, PyObject*>   *m_Globals;     /* 当前栈帧中的全局变量 */

    ArrayList<PyObject*>        *m_FastLocals;

    CodeObject                  *m_Codes;       /* 当前栈帧的CodeObject */
    pc_t                         m_Pc;          /* 程序计数器 类似于x86CPU中的eip */
    Frame                       *m_Caller;      /* 调用当前栈帧的上一个栈帧 */
};

#endif
