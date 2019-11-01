#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>
#include "Core.hpp"
#include "CodeObject.hpp"

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

// public methods
public:
    static Interpreter *get_instance();
    void run(CodeObject* codes);

// private methods
private:
    Interpreter() = default;

// public fields
public:

// private fields
private:
    static Interpreter *m_Instance;
    static std::mutex m_Mutex;

    ArrayList<PyObject*>* m_Stack;    // 运行时栈
    ArrayList<PyObject*>* m_Consts;

};

#endif
