#ifndef FRAME_HPP__
#define FRAME_HPP__

#include <cstdint>

#include "Map.hpp"
#include "Core.hpp"
#include "Block.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"

#include "PyList.hpp"
#include "PyDict.hpp"

class PyFunction;

/**
 * The Implementation of runtime frame in Python Virtual Machine.
 * A&Q:
 * 
 */
class Frame {
public:
    Frame();
    explicit Frame(CodeObject *codes);
    Frame(PyFunction *func, ArrayList<PyObject*> *args, int opArg);
    ~Frame();
public:
    PyList*                    stack()             { return m_Stack;}

    ArrayList<Block*>*         loop_stack()        { return m_LoopStack;}
    ArrayList<PyObject*>*      consts()            { return m_Consts;}
    ArrayList<PyObject*>*      names()             { return m_Names;}

    PyDict*                    locals()            { return m_Locals;}
    PyDict*                    globals()           { return m_Globals;}
    PyList*                    fastLocals()        { return m_FastLocals;}

    void                       set_pc(pc_t pc)     { m_Pc = pc;}
    pc_t                       get_pc()            { return m_Pc;}
    Frame*                     caller()            { return m_Caller;}
    void                       set_caller(Frame *f){ m_Caller = f;}
    bool                       is_first_frame()    { return m_Caller == nullptr;}
    bool                       has_more_codes();
    uint8_t                    get_op_code();
    uint32_t                   get_op_arg();
public:
    PyList                     *m_Stack;
    ArrayList<Block*>          *m_LoopStack;

    ArrayList<PyObject*>       *m_Consts;
    ArrayList<PyObject*>       *m_Names;

    PyDict                     *m_Locals;      /* Locals */
    PyDict                     *m_Globals;     /* Globals */
    PyList                     *m_FastLocals;  /* FastLocal */
    PyList                     *m_Closure;     /* Closure */

    CodeObject                 *m_Codes;       /* The Code to Run */
    Frame                      *m_Caller;      /* Last Frame */
    pc_t                       m_Pc;           /* Program Counter */
};

#endif
