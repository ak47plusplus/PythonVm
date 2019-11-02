#include "VM.cpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"

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
