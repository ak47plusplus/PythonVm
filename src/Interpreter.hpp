#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>
#include <stdint.h>

#include "Core.hpp"
#include "Map.hpp"
#include "Frame.hpp"
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
    Interpreter();
    void eval_frame();
    void exec_new_frame(PyObject *callable, ArrayList<PyObject*> *funcArgs, int opArg);
    void destroy_frame();
    void leave_frame();
private:
    static Interpreter  *       m_Instance;
    static std::mutex           m_Mutex;
    Map<PyObject*,PyObject*>*   m_Builtins;         // 内建对象
    Frame *                     m_CurrentFrame;
    PyObject*                   m_RetValue;


};

#endif
