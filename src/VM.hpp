#ifndef VM_HPP____
#define VM_HPP____

#include "Core.hpp"

class PyInteger;
class PyObject;

class VM {
public:
    static void init() NOEXCEPT;
    static void destroy() NOEXCEPT;
private:

public:
    static PyInteger *PyTrue;
    static PyInteger *PyFalse;
    static PyObject   *PyNone;
};

#endif
