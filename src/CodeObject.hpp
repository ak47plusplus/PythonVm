#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include "PyObject.hpp"
#include "ArrayList.hpp"

class PyString;

class CodeObject : public PyObject {
public:
    explicit CodeObject(
      int argCount,
      int nlocals,
      int stackSize,
      int flag,
      PyString *byteCodes,
      ArrayList<PyObject*> *consts,
      ArrayList<PyObject*> *names,
      ArrayList<PyObject*> *varnames,
      ArrayList<PyObject*> *freevars,
      ArrayList<PyObject*> *cellvars,
      PyString *fileName,
      PyString *coName,
      int lineno,
      PyString *notable);
      ~CodeObject();

public:
    int m_ArgCount;                     /* 参数个数 */
    int m_Nlocals;                      /* 局部变量个数 */
    int m_StackSize;                    /* 当前段code使用的操作数栈的深度的最大值 */
    int m_Flag;                         /* code的属性值 */
    PyString *m_ByteCodes;              /* 字节码 */

    ArrayList<PyObject*> *m_Consts;     /* 常量表 */
    ArrayList<PyObject*> *m_Names;      /* 变量表 */
    ArrayList<PyObject*> *m_Varnames;
    ArrayList<PyObject*> *m_Freevars;
    ArrayList<PyObject*> *m_Cellvars;

    PyString *m_FileName;
    PyString *m_CoName;
    int m_Lineno;
    PyString *m_Notable;
};


#endif
