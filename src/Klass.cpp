#include "Klass.hpp"
#include "PyString.hpp"
#include "Panic.hpp"
#include "VM.hpp"
#include "PyBool.hpp"
#include "PyNoneType.hpp"

Klass::Klass()
= default;

Klass::~Klass()
= default;

PyObject* Klass::toBool(PyObject *x)
{
    return VM::PyFalse;
}