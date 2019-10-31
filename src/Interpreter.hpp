#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <mutex>
#include "CodeObject.hpp"
#include "NonCopyable.hpp"

class Interpreter : public fromboost::NonCopyable {

  enum Status {
      IS_OK,
      IS_BREAK,
      IS_CONTINUE,
      IS_EXCEPTION,
      IS_RETURN,
      IS_YIELD,
  };

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
};

#endif
