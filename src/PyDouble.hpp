#ifndef PY_DOUBLE_HPP___
#define PY_DOUBLE_HPP___

#include "PyObject.hpp"

class PyDouble : public PyObject {
public:
    PyDouble(double val);
    double value() {return m_value;}
private:
    double m_value;
};

#endif
