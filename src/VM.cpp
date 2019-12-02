#include "VM.hpp"
#include "Native.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyFunction.hpp"

#include "Interpreter.hpp"
#include "CodeObject.hpp"

PyInteger * VM::PyTrue = nullptr;
PyInteger * VM::PyFalse = nullptr;
PyObject  * VM::PyNone = nullptr;

void VM::init() NOEXCEPT
{
    VM::PyTrue = new PyInteger(1);
    VM::PyFalse = new PyInteger(0);
    VM::PyNone = new PyObject();
}


void VM::destroy() NOEXCEPT
{

}

void VM::run(CodeObject *mainCode) NOEXCEPT
{
    Interpreter::Get().run(mainCode);
}
