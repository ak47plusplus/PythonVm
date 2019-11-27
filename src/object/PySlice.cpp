#include "PySlice.hpp"

PySlice::PySlice(PyObject *start_, PyObject *stop_, PyObject *step_)
    :start(start_), stop(stop_), step(step_)
{

}