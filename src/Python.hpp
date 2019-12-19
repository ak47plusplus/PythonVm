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
#include "PyTypeObject.hpp"
#include "PyNoneType.hpp"
#include "PyBool.hpp"
#include "PyFunction.hpp"

#define PyObject_IsNullPtr(objPtr)  ((objPtr) == nullptr)
#define PyObject_NotNullPtr(objPtr) ((objPtr) != nullptr)
#define PyObject_IsPyNone(objPtr) PyObject_Klass_Check0(objPtr, NoneTypeKlass)
#define PyObject_Klass_Check(objPtr, klsPtr)  (((objPtr) != nullptr) && ((objPtr)->klass()) == klsPtr)
#define PyObject_Klass_Check0(objPtr, kls)  (((objPtr) != nullptr) && ((objPtr)->klass()) == kls::get_instance())

template<typename P, typename... T>
static inline bool PyObject_Klass_In(P p, T... kls)
{
    bool in = false;
    for(auto x : {args...})
    {
        if(p->klass() == x)
        {
            in = true;
        }
    }
    return in;
}
#endif