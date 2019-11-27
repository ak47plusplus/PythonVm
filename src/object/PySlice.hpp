#ifndef PY_SLICE_HPP_
#define PY_SLICE_HPP_

#include "PyObject.hpp"

struct PySlice : public PyObject {
    PyObject *start, *stop, *step; /* Not Null */

    PySlice(PyObject *start_, PyObject *stop_, PyObject *step_);
};

#endif