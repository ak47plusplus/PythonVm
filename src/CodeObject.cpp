#include "CodeObject.hpp"

CodeObject::CodeObject(
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
  PyString *notable)
  :
  m_ArgCount(argCount),m_Nlocals(nlocals),m_StackSize(stackSize),m_Flag(flag),
  m_ByteCodes(byteCodes),
  m_Consts(consts),m_Names(names),m_Varnames(varnames),m_Freevars(freevars),m_Cellvars(cellvars),
  m_FileName(fileName), m_CoName(coName),m_Lineno(lineno),m_Notable(notable)
  NOEXCEPT
{

}
