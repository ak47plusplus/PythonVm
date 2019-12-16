#include "Interpreter.hpp"
#include "VM.hpp"
#include "OpCode.hpp"
#include "Native.hpp"
#include "Frame.hpp"
#include "Map.hpp"
#include "ArrayList.hpp"
#include "Panic.hpp"
#include "PySlice.hpp"
#include "PyString.hpp"
#include "PyInteger.hpp"
#include "PyTuple.hpp"
#include "PyList.hpp"
#include "PyListIterator.hpp"
#include "PyDict.hpp"
#include "PyFunction.hpp"
#include "PyCell.hpp"
#include "Python.hpp"

#include <mutex>
#include <cstdio>
#include <memory>
#include <iostream>

#define PUSH(v)           m_CurrentFrame->m_Stack->push((v))
#define POP()             m_CurrentFrame->m_Stack->pop()
#define TOP()             m_CurrentFrame->m_Stack->top()
#define STACK_LEVEL()     m_CurrentFrame->m_Stack->size()
#define SET_PC(v)         m_CurrentFrame->set_pc(v)

#define TRUE              VM::PyTrue
#define FALSE             VM::PyFalse
#define None              VM::PyNone

boost::thread_specific_ptr<Interpreter> Interpreter::gs_ThreadLocalInterpreter; /* NOLINT */

Interpreter& Interpreter::Get()
{
   if(!gs_ThreadLocalInterpreter.get())
   {
       gs_ThreadLocalInterpreter.reset(new Interpreter());
   }
   return *gs_ThreadLocalInterpreter;
}

Interpreter::Interpreter()
{
    m_Builtins = new Map<PyObject*, PyObject*>;
    m_Builtins->put(new PyString("True"),  VM::PyTrue);
    m_Builtins->put(new PyString("False"), VM::PyFalse);
    m_Builtins->put(new PyString("None"),  VM::PyNone);

    // native function
    m_Builtins->put(new PyString("id"),  new PyFunction(native::python_builtins::id));
    m_Builtins->put(new PyString("abs"), new PyFunction(native::python_builtins::abs));
    m_Builtins->put(new PyString("len"), new PyFunction(native::python_builtins::len));
}

void Interpreter::Run(CodeObject *codes)
{
    m_CurrentFrame = new Frame(codes);
    EvalFrame();
    DestroyFrame();
}

void Interpreter::EvalFrame()
{
    pc_t pc = 0;
    while (m_CurrentFrame->has_more_codes()) {
        uint8_t opCode = m_CurrentFrame->get_op_code();
        bool hasArgument = (opCode & 0xff) >= OpCode::HAVE_ARGUMENT;
        int opArg = -1;
        if (hasArgument) {
            opArg = m_CurrentFrame->get_op_arg();
        }
        // 获取pc必须在获取opCode和opArg之后
        // 踩坑: 6 SETUP_LOOP   51(to 60)
        pc = m_CurrentFrame->get_pc();

        PyInteger *lhs, *rhs;
        PyObject *v, *w, *u, *x, *attr;
        Block *b;
        PyFunction *func;
        ArrayList<PyObject*> *funcArgs = nullptr;
        ArrayList<PyObject*> *defaultArgs = nullptr;
        PyList *list = nullptr;
        PyTuple *tuple = nullptr;
        int argCount = 0;

        switch (opCode) {
            case OpCode::POP_TOP:
                POP();
                break;
            case OpCode::ROT_TWO:
                v = POP();
                w = POP();
                PUSH(v);
                PUSH(w);
            case OpCode::ROT_THREE:
                break;
            case OpCode::DUP_TOP:
                PUSH(TOP());
                break;
            case OpCode::PRINT_ITEM:     // 71
                v = POP();
                v->print();
                break;
            case OpCode::PRINT_NEWLINE:  // 72
                std::cout << '\n';
                std::fflush(stdout);
                break;
            case OpCode::INPLACE_MULTIPLY:
            case OpCode::BINARY_MULTIPLY: // 20
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;
            case OpCode::INPLACE_DIVIDE:
            case OpCode::BINARY_DIVIDE:  // 21
                v = POP();
                w = POP();
                PUSH(w->div(v));
                break;
            case OpCode::INPLACE_MODULO:
            case OpCode::BINARY_MODULO:  // 22
                v = POP();
                w = POP();
                PUSH(w->mod(v));
                break;
            case OpCode::INPLACE_ADD:         // 55  (+=)
            case OpCode::BINARY_ADD:          // 23  (+)
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;
            case OpCode::INPLACE_SUBSTRACT:   // 56
            case OpCode::BINARY_SUBTRACT:     // 24
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;
            /*
             * 在python3中,所有的切片操作都变成了BUILD_SLICE+BINARYZ_SUBSCR的组合,并废弃了其余
             * 的SLICE相关的字节码。
             * 在python2中也有BUILD_SLICE + BINARY_SUBSCR的组合 但是经过测试 发现python2编译器
             * 只有在切片seq[ilow:ihigh:step]都存在的时候才会编译成这个组合,其余情况都会使用上面的字节码。
             */
            case OpCode::BUILD_SLICE:
                if(opArg == 3) /* opArg always = 3 in python2 */
                    w = POP();
                else
                    w = nullptr;
                v = POP();
                u = TOP(); /* Do not pop the stack */
                //x = new PySlice();
                break;
            case OpCode::SLICE:
            case OpCode::SLICE_1:// 0001
            case OpCode::SLICE_2:// 0010
            case OpCode::SLICE_3:// 0011
                if((opCode-OpCode::SLICE) & 2 /* 0010 */) // SLICE_2 SLICE_3
                    w = POP();
                else
                    w = nullptr;
                if((opCode-OpCode::SLICE) & 1 /* 0001 */) // SLICE_1 SLICE_3
                    v = POP();
                else
                    v = nullptr;
                u = POP();
                // u[v:w]
                // TODO
                break;
            case OpCode::BINARY_SUBSCR:
                // 先压入栈中的是list/str再压入的是下标
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;
            case OpCode::STORE_SUBSCR:
                v = POP();// index   / k
                w = POP();// payload / map
                u = POP();// value   / v
                w->store_subscr(v, u);
                break;
            case OpCode::DELETE_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->del_subscr(v));
                break;
            case OpCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;
            case OpCode::FOR_ITER:
                {
                    v = POP();
                    auto* it = dynamic_cast<Iterator<PyObject*>*>(v);
                    if(it->has_next())
                    {
                       /*
                        * 先需要将迭代器压栈,一次for循环结束后JUMP_ABSOLUTE直接到FOR_ITER指令
                        * 取出迭代器,因此需要在压入迭代元素之前先压入迭代器.
                        */
                        PUSH(v);
                        PUSH(it->next());
                    }
                    else {
                       /*
                        * 当迭代器没有元素时,应该退出for循环.程序计数器应该跳转到POP_BLOCK处
                        * 进入for循环时已经安装了一个BLOCK.
                        */
                        SET_PC(pc + opArg);
                    }
                }
                break;
            case OpCode::RETURN_VALUE:  // 83
                m_RetValue = POP();
                if(m_CurrentFrame->is_first_frame())
                    return;
                LeaveFrame();
                break;
            case OpCode::LOAD_ATTR:
                v = POP();
                w = m_CurrentFrame->m_Names->get(opArg);
                PUSH(v->getattr(w)); // 这里的w和VM初始化的put进去的明显不是同一个,这里承载attr的map肯定不能使用stl里的map
                break;
            case OpCode::LOAD_CONST:      // 100
                PUSH(m_CurrentFrame->m_Consts->get(opArg));
                break;
            case OpCode::STORE_NAME:   // 90
                v = m_CurrentFrame->names()->get(opArg);
                m_CurrentFrame->locals()->put(v, POP());
                break;
            case OpCode::DELETE_NAME:
                v = m_CurrentFrame->names()->get(opArg);
                m_CurrentFrame->locals()->del_key(v);
                break;
            case OpCode::LOAD_NAME:   // 101
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
            case OpCode::STORE_GLOBAL:
                v = m_CurrentFrame->names()->get(opArg);
                m_CurrentFrame->globals()->put(v, POP());
                break;
            case OpCode::LOAD_GLOBAL:
                v = m_CurrentFrame->names()->get(opArg);
                w = m_CurrentFrame->globals()->get(v);
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
            case OpCode::BUILD_TUPLE:
                /* t = (1,2,3)这种在解析字节码的时候就已经生成PyTuple了 这里仅适用于动态生成Tuple
                 * 譬如函数返回多个值时.
                 */
                tuple = new PyTuple();
                while(opArg--)
                {
                    tuple->set(opArg, POP());
                }
                PUSH(tuple);
                break;
            case OpCode::BUILD_LIST:
                list = new PyList();
                while(opArg--)
                {
                    list->set(opArg, POP());
                }
                PUSH(list);
                break;
            case OpCode::BUILD_MAP:
                PUSH(new PyDict(opArg));
                break;
            case OpCode::STORE_MAP:
                v = POP();// k
                w = POP();// v
                u = POP();// dict
                dynamic_cast<PyDict*>(u)->put(v, w);
                PUSH(u);
                break;
            case OpCode::STORE_FAST:
                m_CurrentFrame->fastLocals()->set(opArg, POP());
                break;
            case OpCode::LOAD_FAST:
                PUSH(m_CurrentFrame->fastLocals()->get(opArg));
                break;
            case OpCode::JUMP_FORWARD:        // 110
                m_CurrentFrame->set_pc(pc + opArg);
                break;
            case OpCode::JUMP_ABSOLUTE:       // 113
                m_CurrentFrame->set_pc(opArg);
                break;
            case OpCode::POP_JUMP_IF_FALSE:   // 114
                v = POP();
                if(VM::PyFalse == v)
                    m_CurrentFrame->set_pc(opArg);
                break;
            case OpCode::SETUP_LOOP:          // 120
                m_CurrentFrame->loop_stack()->add(new Block(opCode, pc + opArg, STACK_LEVEL()));
                break;
            case OpCode::POP_BLOCK:
                b = m_CurrentFrame->loop_stack()->pop();
                delete b;
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                break;
            case OpCode::BREAK_LOOP:
                b = m_CurrentFrame->loop_stack()->pop();
                while(STACK_LEVEL() > b->m_Level)
                {
                    POP();
                }
                m_CurrentFrame->set_pc(b->m_Target);
                delete b;
                break;
            case OpCode::MAKE_FUNCTION:
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
            case OpCode::CALL_FUNCTION:
                /* 先压入的是函数本身 然后压入的是函数的参数
                 * add(1,2)
                 * LOAD_NAME    0(add)
                 * LOAD_CONST   1(99)
                 * LOAD_CONST   2(1)
                 * CALL_FUNCTION 2
                 * opArg为2字节,高8位代表键参数的个数,低8位代表位置参数的个数
                 */
                if(opArg > 0)
                {
                    int h8b = opArg & 0xff00;
                    int l8b = opArg & 0xff;
                    argCount = 2 * h8b + l8b;
                    funcArgs = new ArrayList<PyObject*>(argCount);
                    while (argCount--)
                    {
                        funcArgs->set(argCount, POP());
                    }
                }
                v = POP();
                this->EvalNewFrame(v, funcArgs, opArg);
                if(funcArgs != nullptr)
                {
                    delete funcArgs;
                    funcArgs = nullptr;
                    argCount = 0;
                }
                break;
            case OpCode::COMPARE_OP:   // 107
                w = POP();
                v = POP();
                switch (opArg) {
                    case OpCode::GREATER:
                        PUSH(v->greater(w));
                        break;
                    case OpCode::LESS:
                        PUSH(v->less(w));
                        break;
                    case OpCode::EQUAL:
                        PUSH(v->equal(w));
                        break;
                    case OpCode::NOT_EQUAL:
                        PUSH(v->not_equal(w));
                        break;
                    case OpCode::GREATER_EQUAL:
                        PUSH(v->ge(w));
                        break;
                    case OpCode::LESS_EQUAL:
                        PUSH(v->le(w));
                        break;
                    case OpCode::IS:
                        PUSH(v == w ? VM::PyTrue : VM::PyFalse);
                        break;
                    case OpCode::IS_NOT:
                        PUSH(v != w ? VM::PyTrue : VM::PyFalse);
                        break;
                    case OpCode::IN:
                        PUSH(w->contains(v));
                        break;
                    default:
                        __panic("Unrecognized compare op arg: %d\n", opArg);
                }
                break;
            case OpCode::STORE_DEREF:
                // 如果一个变量会被内部函数捕获形成闭包 那么编译成的字节码不是STORE_NAME
                // 也不是STORE_FAST, 而是STORE_DEREF
                m_CurrentFrame->m_Closure->set(opArg, POP());
                break;
            case OpCode::LOAD_DEREF:
                v = m_CurrentFrame->m_Closure->get(opArg);
                if(PyObject_Klass_Check0(v, CellKlass))
                {
                    v = ((PyCell*)v)->get();
                }
                PUSH(v);
                break;
            case OpCode::LOAD_CLOSURE:
                v = m_CurrentFrame->m_Closure->get(opArg);
                if(v == nullptr) // set cause not nullptr
                {
                    // 说明不是局部变量, 而是在参数中的
                    v = m_CurrentFrame->get_cell_from_func_arguments(opArg);
                    m_CurrentFrame->m_Closure->set(opArg, v);
                }
                // 这里的判断适用于多层闭包的情况下.
                if(PyObject_Klass_Check0(v, CellKlass))
                    PUSH(v);
                else
                    PUSH(new PyCell(m_CurrentFrame->m_Closure, opArg));
                break;
            case OpCode::MAKE_CLOSURE:
                v = POP();
                func = new PyFunction((CodeObject*)v);
                func->set_closure((PyTuple*)(POP()));
                func->set_globals(m_CurrentFrame->globals());
                // default args
                if(opArg > 0) {
                    defaultArgs = new ArrayList<PyObject*>(opArg);
                    while (opArg--) {
                        defaultArgs->set(opArg, POP());
                    }
                    func->set_default_args(defaultArgs); // made copy
                    delete defaultArgs; // so it can be delete.
                    defaultArgs = nullptr;
                }
                PUSH(func);
                break;
            default:
                __panic("Unsupported opCode: %d \n", opCode);
        }
    }
}

/**
 * Destroy current frame, and switch the current frame to the super.
 */
void Interpreter::DestroyFrame()
{
    Frame *frame_to_destroy = m_CurrentFrame;
    m_CurrentFrame = frame_to_destroy->caller();
    delete frame_to_destroy;
}

/**
 * leave current frame and give back the ownership of Interpter to the super frame.
 * <p>
 * Record the return value first, then destroy current frame, and push the return value
 * to the top of the super frame stack.
 */
void Interpreter::LeaveFrame()
{
    this->DestroyFrame();
    PUSH(m_RetValue);
}

/**
 * Make a new stack frame and takes the ownership of the Interpter.
 * @param callable The Function Object
 * @param funcArgs The list of function args.
 * @param opArg OpCode (CALL_FUNCTION) opArg
 */
void Interpreter::EvalNewFrame(PyObject *callable, ArrayList<PyObject*> *funcArgs, int opArg)
{
    /* Native function Call. */
    if(PyObject_Klass_Check0(callable, NativeFunctionKlass))
    {
        PUSH(dynamic_cast<PyFunction*>(callable)->native_call(funcArgs));
    }
    /* Futures Method Call of a Class */
    else if(PyObject_Klass_Check0(callable, MethodKlass)) 
    {
        ArrayList<PyObject*> selfArg(1);
        auto *method = dynamic_cast<PyMethod*>(callable);
        if(funcArgs == nullptr) {
            funcArgs = &selfArg;
        }
        funcArgs->insert(0, method->owner());
        EvalNewFrame(method->func(), funcArgs, opArg + 1); // opArg+1应该不影响高八位?
    }
    /* Simple function Call of Python */
    else 
    {
        auto *newFrame = new Frame((PyFunction*)callable, funcArgs, opArg);
        newFrame->set_caller(m_CurrentFrame);
        m_CurrentFrame = newFrame;
    }
}