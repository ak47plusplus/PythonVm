#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>
#include <stdint.h>

#include "Core.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"

// 用于记录循环的block 主要用于多层嵌套循环和break语句使用
class Block {
public:
    uint8_t    m_Type;      // block的类型 除了wile还有except还有finnally等
    uint32_t   m_Target;    // 跳转的目标地址
    int32_t    m_Level;     // 进入block时 操作数栈的深度 对while block影响不大.

    Block()
    {
        m_Type = 0;
        m_Target = 0;
        m_Level = 0;
    }

    Block(uint8_t _type, uint32_t _target, int32_t _level)
        :m_Type(_type),m_Target(_target),m_Level(level){}

    Block(const Block &rhs)
    {
        m_Type = rhs.m_Type;
        m_Target = rhs.m_Target;
        m_Level = rhs.m_Level;
    }
};

class Interpreter {

  enum Status {
      IS_OK,
      IS_BREAK,
      IS_CONTINUE,
      IS_EXCEPTION,
      IS_RETURN,
      IS_YIELD,
  };

DISABLE_COPY(Interpreter)

public:
    static Interpreter *get_instance();
    void run(CodeObject* codes);
private:
    Interpreter() = default;
private:
    static Interpreter *m_Instance;
    static std::mutex m_Mutex;
    ArrayList<PyObject*>* m_Stack;    // 运行时栈 解释具体的CodeObject时进行创建，解释完毕直接销毁
    ArrayList<PyObject*>* m_Consts;   // 指向当前执行的CodeObject的常量表.

};

#endif
