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
{

}

CodeObject::~CodeObject()
{
    if(m_ByteCodes != nullptr)
        delete m_ByteCodes;
    if(m_Consts != nullptr)
        delete m_Consts;
    if(m_Names != nullptr)
        delete m_Names;
    if(m_Varnames != nullptr)
        delete m_Varnames;
    if(m_Freevars != nullptr)
        delete m_Freevars;
    if(m_Cellvars != nullptr)
        delete m_Cellvars;
    if(m_FileName != nullptr)
        delete m_FileName;
    if(m_CoName != nullptr)
        delete m_CoName;
    if(m_Notable != nullptr)
        delete m_Notable;
}
