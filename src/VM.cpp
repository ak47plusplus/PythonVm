#include "VM.hpp"
#include "Native.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyFunction.hpp"

PyInteger * VM::PyTrue = nullptr;
PyInteger * VM::PyFalse = nullptr;
PyObject  * VM::PyNone = nullptr;

void VM::init() NOEXCEPT
{
    VM::PyTrue = new PyInteger(1);
    VM::PyFalse = new PyInteger(0);
    VM::PyNone = new PyObject();

    // init string.
    StringKlass::get_instance()->register_klass_dict(new PyString("upper"), new PyFunction(pystring::string_upper));
    // init list.
    ListKlass::get_instance()->register_klass_dict(new PyString("append"), new PyFunction(pylist::list_append));
}


void VM::destroy() NOEXCEPT
{

}
