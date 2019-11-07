#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>
#include <stdint.h>

#include "Core.hpp"
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
    Interpreter() = default;
private:
    static Interpreter *  m_Instance;
    static std::mutex     m_Mutex;

    Frame *               m_Frame;

};

#endif
