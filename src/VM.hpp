#ifndef VM_HPP____
#define VM_HPP____

#include "Core.hpp"
#include "CodeObject.hpp"

class PyBool;
class PyNoneType;

class VM {
public:
    static void init() NOEXCEPT;
    static void destroy() NOEXCEPT;
    static void run(CodeObject *mainCode) NOEXCEPT;
private:

public:
    static PyBool     *PyTrue;
    static PyBool     *PyFalse;
    static PyNoneType *PyNone;
};

#define TRUE    (VM::PyTrue)
#define FALSE   (VM::PyFalse)
#define NONE    (VM::PyNone)

#endif
