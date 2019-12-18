#include "VM.hpp"
#include "Python.hpp"
#include "Native.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "PyFunction.hpp"
#include "PyNoneType.hpp"

#include "Interpreter.hpp"
#include "CodeObject.hpp"

PyBool  *VM::PyTrue = nullptr;
PyBool  *VM::PyFalse = nullptr;
PyNoneType *VM::PyNone = nullptr;

void VM::init() NOEXCEPT
{
    // type klass object之间有相互的循环引用 需要手动设置关系.
    PyTypeObject *typeObjForTypeKlass = new PyTypeObject();
    typeObjForTypeKlass->set_own_klass(TypeKlass::get_instance());

    PyTypeObject *typeObjForObjectKlass = new PyTypeObject();
    typeObjForObjectKlass->set_own_klass(ObjectKlass::get_instance());

    TypeKlass::get_instance()->set_super(ObjectKlass::get_instance());


    IntegerKlass::get_instance()->InitKlass();
    DoubleKlass::get_instance()->InitKlass();
    ListKlass::get_instance()->InitKlass();
    DictKlass::get_instance()->InitKlass();
    StringKlass::get_instance()->InitKlass();
    NoneTypeKlass::get_instance()->InitKlass();
    BoolKlass::get_instance()->InitKlass();

    TypeKlass::get_instance()->set_attrs(new PyDict());
    ObjectKlass::get_instance()->set_attrs(new PyDict());

    VM::PyTrue = new PyBool(true);
    VM::PyFalse = new PyBool(false);
    VM::PyNone = new PyNoneType();
}


void VM::destroy() NOEXCEPT
{

}

void VM::run(CodeObject *mainCode) NOEXCEPT
{
    Interpreter::Get().Run(mainCode);
}
