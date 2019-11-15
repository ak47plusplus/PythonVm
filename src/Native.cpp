#include "Native.hpp"
#include "PyFunction.hpp"
#include "PyInteger.hpp"
#include "PyString.hpp"
#include "PyDouble.hpp"
#include "Panic.hpp"
#include <cmath>
#include <cassert>

namespace native {

/**
 * namespace python_builtins: python的内建函数.
 * 在无任何常量声明或者方法声明时，可以直接在python代码中使用
 */
namespace python_builtins {

    /**
     * 获取一个对象的唯一id.
     * 在本虚拟机中返回的是对象的地址.
     */
    PyObject* id(FuncArgs args)
    {
        if(args == nullptr || args->size() != 1)
        {
            __panic("TypeError: id() takes exactly one argument(%d given)", args == nullptr ? 0 : args->size());
        }
        return args->get(0)->id();
    }

    /**
     * 对一个整形或者浮点型求绝对值
     */
    PyObject* abs(FuncArgs args)
    {
        PyObject *arg = args->get(0);
        if(arg->klass() == IntegerKlass::get_instance())
        {
            return new PyInteger(std::abs(dynamic_cast<PyInteger*>(arg)->value()));
        }
        else if(arg->klass() == DoubleKlass::get_instance())
        {
            return new PyDouble(std::fabs(dynamic_cast<PyDouble*>(arg)->value()));
        } else
        {
            __panic("TypeError: bad operand type for abs():'%s' ^_^\n", "?");
        }
        return nullptr;
    }

    /**
     * 对入参求长度.具体的逻辑取决于具体的对象类型.
     * 譬如如果是PyString则返回的是字符串的长度
     * 如果是PyList则返回列表的元素个数.
     */
    PyObject* len(FuncArgs args)
    {
        return args->get(0)->len();
    }

    PyObject* type_of(FuncArgs args)
    {
        return nullptr;
    }

    PyObject* isinstance(FuncArgs args)
    {
        return VM::PyFalse;
    }
} // end of namespace python_builtins.


} // end of namespace native
