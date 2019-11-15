#ifndef NATIVE_HPP_
#define NATIVE_HPP_

#include "PyObject.hpp"
#include "ArrayList.hpp"

namespace native {

typedef ArrayList<PyObject*> *FuncArgs;

namespace python_builtins {
    extern PyObject* id(FuncArgs args);
    extern PyObject* abs(FuncArgs args);
    extern PyObject* len(FuncArgs args);
    extern PyObject* type_of(FuncArgs args);
    extern PyObject* isinstance(FuncArgs args);
}
}


#endif
