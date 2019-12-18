#ifndef PYTHON_HPP_
#define PYTHON_HPP_

#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include "PyString.hpp"
#include "PyList.hpp"
#include "PyTuple.hpp"
#include "PySlice.hpp"
#include "PyDict.hpp"
#include "PyFunction.hpp"
#include "PyTypeObject.hpp"

#define PyObject_NotNullPtr(objPtr) ((objPtr) != nullptr)
#define PyObject_Klass_Check(objPtr, klsPtr)  (((objPtr) != nullptr) && ((objPtr)->klass()) == klsPtr)
#define PyObject_Klass_Check0(objPtr, kls)  (((objPtr) != nullptr) && ((objPtr)->klass()) == kls::get_instance())

#endif