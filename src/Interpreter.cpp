#include "Interpreter.hpp"
#include "VM.hpp"
#include "Map.hpp"
#include "Panic.hpp"
#include "ByteCode.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"
#include "Frame.hpp"
#include "PyFunction.hpp"

#include <mutex>
#include <cstdio>
#include <memory>
#include <iostream>

#define PUSH(v)         m_CurrentFrame->m_Stack->add((v))
#define POP()           m_CurrentFrame->m_Stack->pop()
#define STACK_LEVEL()   m_CurrentFrame->m_Stack->size()
#define SET_PC(v)       m_CurrentFrame->set_pc(v)

#define TRUE            VM::PyTrue
#define FALSE           VM::PyFalse
#define None            VM::PyNone

Interpreter* Interpreter::m_Instance = nullptr;
std::mutex Interpreter::m_Mutex;

Interpreter *Interpreter::get_instance()
{
    if (Interpreter::m_Instance == nullptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(Interpreter::m_Instance == nullptr) {
            Interpreter::m_Instance = new Interpreter();
        }
    }
    return Interpreter::m_Instance;
}

Interpreter::Interpreter()
{
    m_Builtins = new Map<PyObject*, PyObject*>;
    m_Builtins->put(new PyString("True"), VM::PyTrue);
    m_Builtins->put(new PyString("False"), VM::PyFalse);
    m_Builtins->put(new PyString("None"), VM::PyNone);

    // native function
    m_Builtins->put(new PyString("abs"), new PyFunction(native::abs));
    m_Builtins->put(new PyString("len"), new PyFunction(native::len));
}

void Interpreter::run(CodeObject *codes)
{
    m_CurrentFrame = new Frame(codes);
    eval_frame();
    destroy_frame();
}


void Interpreter::eval_frame()
{
    pc_t pc = 0;
    while (m_CurrentFrame->has_more_codes()) {
        uint8_t opCode = m_CurrentFrame->get_op_code();
        // printf("\n===> start to parse opCode, opCode nbr: %d \n", opCode);
        bool hasArgument = (opCode & 0xff) >= ByteCode::HAVE_ARGUMENT;
        int opArg = -1;
        if (hasArgument) {
            opArg = m_CurrentFrame->get_op_arg();
        }
        // 获取pc必须在获取opCode和opArg之后
        // 踩坑: 6 SETUP_LOOP   51(to 60)
        pc = m_CurrentFrame->get_pc();

        PyInteger *lhs, *rhs;
        PyObject *v, *w, *u, *attr;
        Block *b;
        PyFunction *func;
        ArrayList<PyObject*> *funcArgs = nullptr;
        ArrayList<PyObject*> *defaultArgs = nullptr;
        int argCount = 0;

        switch (opCode) {
            case ByteCode::POP_TOP:
                POP();
                break;
            case ByteCode::PRINT_ITEM:     // 71
                v = POP();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:  // 72
                std::cout << '\n';
                std::fflush(stdout);
                break;
            case ByteCode::INPLACE_MULTIPLY:
            case ByteCode::BINARY_MULTIPLY: // 20
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;
            case ByteCode::INPLACE_DIVIDE:
            case ByteCode::BINARY_DIVIDE:  // 21
                v = POP();
                w = POP();
                PUSH(w->div(v));
                break;
            case ByteCode::INPLACE_MODULO:
            case ByteCode::BINARY_MODULO:  // 22
                v = POP();
                w = POP();
                PUSH(w->mod(v));
                break;
            case ByteCode::INPLACE_ADD:         // 55  (+=)
            case ByteCode::BINARY_ADD:          // 23  (+)
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;
            case ByteCode::INPLACE_SUBSTRACT:   // 56
            case ByteCode::BINARY_SUBTRACT:     // 24
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;
            case ByteCode::RETURN_VALUE:  // 83
                m_RetValue = POP();
                if(m_CurrentFrame->is_first_frame())
                    return;
                leave_frame();
                break;
            case ByteCode::LOAD_CONST:      // 100
                PUSH(m_CurrentFrame->m_Consts->get(opArg));
                break;
            case ByteCode::STORE_NAME:   // 90
                v = m_CurrentFrame->names()->get(opArg);
                m_CurrentFrame->locals()->put(v, POP());
                break;
            case ByteCode::LOAD_NAME:   // 101
                v = m_CurrentFrame->names()->get(opArg);
                w = m_CurrentFrame->locals()->get(v);
                if(w != VM::PyNone)
                {
                    PUSH(w);
                    break;
                }
                // LOAD_NAME如果从局部变量表找不到则去全局常量表找
                w = m_CurrentFrame->globals()->get(v);
                if(w != VM::PyNone)
                {
                    PUSH(w);
                    break;
                }
                // 如果全局变量表仍然找不到 则去找builtin
                w = m_Builtins->get(v);
                if(w != VM::PyNone)
                {
                    PUSH(w);
                    break;
                }
                PUSH(VM::PyNone);
                break;
            case ByteCode::STORE_GLOBAL:
                v = m_CurrentFrame->names()->get(opArg);
                m_CurrentFrame->globals()->put(v, POP());
                break;
            case ByteCode::LOAD_GLOBAL:
                v = m_CurrentFrame->names()->get(opArg);
                w = m_CurrentFrame->globals()->get(w);
                if(w != VM::PyNone)
                {
                    PUSH(w);
                    break;
                }
                // 全局变量表找不到则尝试去builtin找
                w = m_Builtins->get(v);
                if(w != VM::PyNone)
                {
                    PUSH(w);
                    break;
                }
                PUSH(VM::PyNone);
                break;
            case ByteCode::STORE_FAST:
                m_CurrentFrame->fastLocals()->set(opArg, POP());
                break;
            case ByteCode::LOAD_FAST:
                PUSH(m_CurrentFrame->fastLocals()->get(opArg));
                break;
            case ByteCode::JUMP_FORWARD:        // 110
                m_CurrentFrame->set_pc(pc + opArg);
                break;
            case ByteCode::JUMP_ABSOLUTE:       // 113
                m_CurrentFrame->set_pc(opArg);
                break;
            case ByteCode::POP_JUMP_IF_FALSE:   // 114
                v = POP();
                if(VM::PyFalse == v)
                    m_CurrentFrame->set_pc(opArg);
                break;
            case ByteCode::SETUP_LOOP:          // 120
                m_CurrentFrame->loop_stack()->add(new Block(opCode, pc + opArg, STACK_LEVEL()));
                break;
            case ByteCode::POP_BLOCK:
                b = m_CurrentFrame->loop_stack()->pop();
                delete b;
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                break;
            case ByteCode::BREAK_LOOP:
                b = m_CurrentFrame->loop_stack()->pop();
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                m_CurrentFrame->set_pc(b->m_Target);
                delete b;
                break;
            case ByteCode::MAKE_FUNCTION:
                /*
                 * 先压入的是函数的默认参数 在压入的函数的CodeObject.
                 */
                v = POP();
                func = new PyFunction((CodeObject*)v);
                func->set_globals(m_CurrentFrame->globals());
                if(opArg > 0) // 说明有默认值
                {
                    defaultArgs = new ArrayList<PyObject*>(opArg);
                    while (opArg--)
                    {
                        defaultArgs->set(opArg, POP());
                    }
                }
                func->set_default_args(defaultArgs); // made copy
                if(defaultArgs != nullptr)
                {
                    delete defaultArgs;
                    defaultArgs = nullptr;
                }
                PUSH(func);
                break;
            case ByteCode::CALL_FUNCTION:
                /*
                 * 先压入的是函数本身 然后压入的是函数的参数
                 * add(1,2)
                 * LOAD_NAME    0(add)
                 * LOAD_CONST   1(99)
                 * LOAD_CONST   2(1)
                 * CALL_FUNCTION 2
                 */
                if(opArg > 0)
                {
                    funcArgs = new ArrayList<PyObject*>();
                    argCount = opArg;
                    while (argCount--)
                    {
                        funcArgs->set(argCount, POP());
                    }
                }
                v = POP();
                this->exec_new_frame(v, funcArgs, opArg);
                if(funcArgs != nullptr)
                {
                    delete funcArgs;
                    funcArgs = nullptr;
                    argCount = 0;
                }
                break;
            case ByteCode::COMPARE_OP:   // 107
                w = POP();
                v = POP();
                // COMPARE_OP是带有参数的操作码
                switch (opArg) {
                    case ByteCode::GREATER:
                        PUSH(v->greater(w));
                        break;
                    case ByteCode::LESS:
                        PUSH(v->less(w));
                        break;
                    case ByteCode::EQUAL:
                        PUSH(v->equal(w));
                        break;
                    case ByteCode::NOT_EQUAL:
                        PUSH(v->not_equal(w));
                        break;
                    case ByteCode::GREATER_EQUAL:
                        PUSH(v->ge(w));
                        break;
                    case ByteCode::LESS_EQUAL:
                        PUSH(v->le(w));
                        break;
                    case ByteCode::IS:
                        PUSH(v == w ? VM::PyTrue : VM::PyFalse);
                        break;
                    case ByteCode::IS_NOT:
                        PUSH(v != w ? VM::PyTrue : VM::PyFalse);
                        break;
                    default:
                        __panic("Unrecognized compare op arg: %d\n", opArg);
                }
                break;
            default:
                __panic("Unsupported opCode: %d \n", opCode);
        }
    }
}

/**
 * 销毁当前栈帧.
 * <p>
 * 销毁前需要将当前程序的运行时栈帧设置为被销毁的caller,然后再销毁本身.
 */
void Interpreter::destroy_frame()
{
    Frame *frame_to_destroy = m_CurrentFrame;
    m_CurrentFrame = frame_to_destroy->caller();
    delete frame_to_destroy;
}

/**
 * 离开当前栈帧
 * <p>
 * 在记录了当前栈帧的返回值后,再销毁当前栈帧,将返回值压入调用者栈帧的栈中.
 */
void Interpreter::leave_frame()
{
    this->destroy_frame();
    PUSH(m_RetValue);
}

/**
 * @brief 创建一个新的栈帧 并设置为当前的运行时栈帧.
 * <p>
 * a = 10
 * foo(a)
 * a在主线程运行，随即遇到一个函数调用，则创建一个新的函数栈帧,将当前运行时
 * 栈帧设置为foo函数运行的栈帧.
 * @param callable PyFunction对象
 * @param funcArgs函数的参数列表
 */
void Interpreter::exec_new_frame(PyObject *callable, ArrayList<PyObject*> *funcArgs, int opArg)
{
    // 如果是native函数,直接调用c++写好的函数
    if(callable->klass() == NativeFunctionKlass::get_instance())
    {
        PUSH(dynamic_cast<PyFunction*>(callable)->native_call(funcArgs));
    } else {
        Frame *_new_frame = new Frame((PyFunction*)callable, funcArgs, opArg);
        _new_frame->set_caller(m_CurrentFrame);
        m_CurrentFrame = _new_frame;
    }
}
