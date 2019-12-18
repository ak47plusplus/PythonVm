#ifndef VM_HPP____
#define VM_HPP____

#include "Core.hpp"
#include "CodeObject.hpp"

class PyInteger;
class PyNoneType;

class VM {
public:
    static void init() NOEXCEPT;
    static void destroy() NOEXCEPT;
    static void run(CodeObject *mainCode) NOEXCEPT;
private:

public:
    static PyInteger  *PyTrue;
    static PyInteger  *PyFalse;
    static PyNoneType *PyNone;
};

#endif
