#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>

#include "Core.hpp"
#include "CodeObject.hpp"
#include "ArrayList.hpp"

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
