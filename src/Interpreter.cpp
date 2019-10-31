#include "Interpreter.hpp"
#include "ByteCode.hpp"
#include <mutex>
#include <cstdio>
#include <iostream>

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

    // 循环读取并解析字节码
    while (pc < codeLength) {
        unsigned char opCode = codes->m_ByteCodes->value()[pc++];
        // python的字节码在设计的时候进行了安排，字节码<=90代表无参数的
        bool hasArgument = (opCode & 0xff) > ByteCode::HAVE_ARGUMENT;

        int opArg = -1;
        if (hasArgument) {
            // TODO.
        }

        PyInteger *lhs, *rhs;
        PyObject *v, *w, *u, *attr;

        switch (opCode) {
            case ByteCode::LOAD_CONST:
                m_Stack->add(m_Consts->get(opArg));
                break;
            case ByteCode::PRINT_ITEM:
                v = m_Stack->pop();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:
                std::cout << '\n';
                std::fflush(stdout);
                break;
            case ByteCode::BINARY_ADD:
                v = m_Stack->pop();
                w = m_Stack->pop();
                m_Stack->add(w->add(v));
                break;
            case ByteCode::RETURN_VALUE:
                m_Stack->pop(); // ? just pop ?
                break;
        }
    }
    delete m_Stack;
}
