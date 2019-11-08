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

        switch (opCode) {
            case ByteCode::LOAD_CONST:      // 100
                PUSH(m_CurrentFrame->m_Consts->get(opArg));
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
                m_RetValue = POP(); // ? just pop ?
                if(m_CurrentFrame->is_first_frame())
                    return;
                leave_frame();
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
                PUSH(VM::PyNone);
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
                    default:
                        __panic("Unrecognized compare op arg: %d\n", opArg);
                }
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
                v = POP();
                func = new PyFunction((CodeObject*)v);
                PUSH(v);
                break;
            case ByteCode::CALL_FUNCTION:
                v = POP();
                this->exec_new_frame(v);
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
 * 创建一个新的栈帧 并设置为当前的运行时栈帧.
 * <p>
 * a = 10
 * foo(a)
 * a在主线程运行，随即遇到一个函数调用，则创建一个新的函数栈帧,将当前运行时
 * 栈帧设置为foo函数运行的栈帧.
 */
void Interpreter::exec_new_frame(PyObject *callable)
{
    Frame *_new_frame = new Frame((PyFunction*)callable);
    _new_frame->set_caller(m_CurrentFrame);
    m_CurrentFrame = _new_frame;
}
