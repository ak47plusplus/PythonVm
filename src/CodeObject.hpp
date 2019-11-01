#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include "PyObject.hpp"
#include "ArrayList.hpp"

#include "PyString.hpp"
// class PyString; 无法调用delete

/*
 * @brief 每一个.pyc文件对应的结构
 * @Created by GuoXiang 2019/11/1
 */
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
    PyString *m_ByteCodes;              /* 字节码指令序列 */

    ArrayList<PyObject*> *m_Consts;     /* 常量表 */
    ArrayList<PyObject*> *m_Names;      /* 符号名称集合 */
    ArrayList<PyObject*> *m_Varnames;   /* 局部变量集合 */
    ArrayList<PyObject*> *m_Freevars;   /* 自由变量集合 闭包专用 */
    ArrayList<PyObject*> *m_Cellvars;   /* 内部嵌套函数引用变量名集合*/

    PyString *m_FileName;               /* 代码所在文件名 */
    PyString *m_CoName;                 /* 模块|函数|类名*/
    int m_Lineno;                       /* 代码块在文件中的起始行号 */
    PyString *m_Notable;                /* 字节码指令和行号的对应关系 */
};


#endif
