#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "boost/noncopyable.hpp"
#include "boost/thread/tss.hpp"

#include "Map.hpp"
#include "Frame.hpp"
#include "CodeObject.hpp"

template<typename _Tp>
class ArrayList;

class Interpreter : public boost::noncopyable {

  enum Status {
      IS_OK,
      IS_BREAK,
      IS_CONTINUE,
      IS_EXCEPTION,
      IS_RETURN,
      IS_YIELD,
  };

public:
    static Interpreter& Get();
    void run(CodeObject* codes);
private:
    Interpreter();
    void eval_frame();
    void exec_new_frame(PyObject *callable, ArrayList<PyObject*> *funcArgs, int opArg);
    void destroy_frame();
    void leave_frame();
private:
    /*
     * Here i dont want that just a single instance interpreter object in the vm process.
     * my idea of python threads is that a thread mapping to the posix thread. and each
     * thread need a thread local interpreter.
     */
    static boost::thread_specific_ptr<Interpreter> gs_ThreadLocalInterpreter;

    Map<PyObject*,PyObject*>*   m_Builtins;
    Frame *                     m_CurrentFrame;
    PyObject*                   m_RetValue;
};

#endif
