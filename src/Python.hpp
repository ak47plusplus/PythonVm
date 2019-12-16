#ifndef PYTHON_HPP_
#define PYTHON_HPP_

#include "object/PyObject.hpp"
#include "object/PyInteger.hpp"
#include "object/PyDouble.hpp"
#include "object/PyString.hpp"
#include "object/PyList.hpp"
#include "object/PyTuple.hpp"
#include "object/PySlice.hpp"
#include "object/PyDict.hpp"
#include "object/PyFunction.hpp"

#define PyObject_NotNullPtr(objPtr) ((objPtr) != nullptr)
#define PyObject_Klass_Check(objPtr, klsPtr)  (((objPtr) != nullptr) && ((objPtr)->klass()) == klsPtr)
#define PyObject_Klass_Check0(objPtr, kls)  (((objPtr) != nullptr) && ((objPtr)->klass()) == kls::get_instance())

#endif