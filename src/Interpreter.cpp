#include <mutex>
#include <cstdio>
#include <iostream>

#include "Interpreter.hpp"
#include "VM.hpp"
#include "Panic.hpp"
#include "ByteCode.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"

#define PUSH(v) m_Stack->add((v))
#define POP()   m_Stack->pop()


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
    int pc = 0;   // 指令计数器
    int codeLength = codes->m_ByteCodes->length();
    m_Stack = new ArrayList<PyObject*>(codes->m_StackSize);
    m_Consts = codes->m_Consts;

    // 循环读取并解析字节码 opCode占一个字节 如果有参数 参数占2字节
    while (pc < codeLength) {
        unsigned char opCode = codes->m_ByteCodes->value()[pc++];
        // printf("\nstart to parse opCode, opCode nbr: %d \n", opCode);

        // python的字节码在设计的时候进行了安排，字节码<=90代表无参数的
        bool hasArgument = (opCode & 0xff) > ByteCode::HAVE_ARGUMENT;

        int opArg = -1;
        if (hasArgument) {
            int lowBit = (codes->m_ByteCodes->value()[pc++] & 0xff);
            opArg = ((codes->m_ByteCodes->value()[pc++] & 0xff) << 8) | lowBit;
        }

        PyInteger *lhs, *rhs;
        PyObject *v, *w, *u, *attr;

        switch (opCode) {
            case ByteCode::LOAD_CONST:
                PUSH(m_Consts->get(opArg));
                break;
            case ByteCode::PRINT_ITEM:
                v = POP();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:
                std::cout << '\n';
                std::fflush(stdout);
                break;
            case ByteCode::BINARY_MULTIPLY: // 20
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;
            case ByteCode::BINARY_DIVIDE:  // 21
                v = POP();
                w = POP();
                PUSH(w->div(v));
                break;
            case ByteCode::BINARY_MODULO:  // 22
                v = POP();
                w = POP();
                PUSH(w->mod(v));
                break;
            case ByteCode::BINARY_ADD:      // 23
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;
            case ByteCode::BINARY_SUBTRACT: // 24
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;
            case ByteCode::RETURN_VALUE:  // 83
                POP(); // ? just pop ?
                break;
            case ByteCode::COMPARE_OP:
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
            case ByteCode::JUMP_FORWARD:
                pc += opArg;
                break;
            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                if(VM::PyFalse == v)
                    pc = opArg;
                break;
            default:
                __panic("Unsupported opCode: %c \n", opCode);
        }
    }
    delete m_Stack;
}
