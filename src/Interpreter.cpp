#include "Interpreter.hpp"
#include "VM.hpp"
#include "Map.hpp"
#include "Panic.hpp"
#include "ByteCode.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"

#include <mutex>
#include <cstdio>
#include <iostream>

#define PUSH(v)         m_Stack->add((v))
#define POP()           m_Stack->pop()
#define STACK_LEVEL()   m_Stack->size()

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
    // pc为指令计数器 类似于x86CPU中的eip
    int pc = 0;
    int codeLength = codes->m_ByteCodes->length();

    // 解释字节码的操作栈.
    m_Stack = new ArrayList<PyObject*>(codes->m_StackSize);

    // 常量表
    m_Consts = codes->m_Consts;

    // 符号表 包括变量 常量等的名字.
    ArrayList<PyObject*>        *names = codes->m_Names;

    // 存储变量的名和值的映射表
    Map<PyObject*, PyObject*>   *locals = new Map<PyObject*,PyObject*>();

    // 存储(多层)循环/异常/Finarlly块的栈
    // 例如多层循环时,最外城循环的信息被压入栈的最低端.
    ArrayList<Block*>           *loopStack = new ArrayList<Block*>();

    // 循环读取并解析字节码 opCode占一个字节 如果有参数 参数占2字节
    while (pc < codeLength) {
        unsigned char opCode = codes->m_ByteCodes->value()[pc++];
        // printf("\n===> start to parse opCode, opCode nbr: %d \n", opCode);

        // python的字节码在设计的时候进行了安排，字节码<90代表无参数的
        bool hasArgument = (opCode & 0xff) >= ByteCode::HAVE_ARGUMENT;

        int opArg = -1;
        if (hasArgument) {
            int lowBit = (codes->m_ByteCodes->value()[pc++] & 0xff);
            opArg = ((codes->m_ByteCodes->value()[pc++] & 0xff) << 8) | lowBit;
        }

        PyInteger *lhs, *rhs;
        PyObject *v, *w, *u, *attr;
        Block *b;

        switch (opCode) {
            case ByteCode::LOAD_CONST:      // 100
                PUSH(m_Consts->get(opArg));
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
                POP(); // ? just pop ?
                break;
            case ByteCode::STORE_NAME:   // 90
                v = names->get(opArg);
                locals->put(v, POP());
                break;
            case ByteCode::LOAD_NAME:   // 101
                v = names->get(opArg);
                w = locals->get(v);
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
                pc += opArg;
                break;
            case ByteCode::JUMP_ABSOLUTE:       // 113
                pc = opArg; // 绝对跳转
                break;
            case ByteCode::POP_JUMP_IF_FALSE:   // 114
                v = POP();
                if(VM::PyFalse == v)
                    pc = opArg;
                break;
            case ByteCode::SETUP_LOOP:          // 120
                loopStack->add(new Block(opCode, pc + opArg, STACK_LEVEL()));
                break;
            case ByteCode::POP_BLOCK:
                b = loopStack->pop();
                delete b;
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                break;
            case ByteCode::BREAK_LOOP:
                b = loopStack->pop();
                delete b;
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                pc = b->m_Target;
                break;
            default:
                __panic("Unsupported opCode: %d \n", opCode);
        }
    }
    delete m_Stack;
}
