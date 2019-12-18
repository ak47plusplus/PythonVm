#include "VM.hpp"
#include "Python.hpp"
#include "Native.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
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

    // type object was dependency circle.
    Klass *typeKlass = TypeKlass::get_instance();
    PyTypeObject *pTypeObject = new PyTypeObject();
    typeKlass->set_type_object(pTypeObject);

    IntegerKlass::get_instance()->InitKlass();
    DoubleKlass::get_instance()->InitKlass();
    ListKlass::get_instance()->InitKlass();
    DictKlass::get_instance()->InitKlass();
    StringKlass::get_instance()->InitKlass();
}


void VM::destroy() NOEXCEPT
{

}

void VM::run(CodeObject *mainCode) NOEXCEPT
{
    Interpreter::Get().Run(mainCode);
}
