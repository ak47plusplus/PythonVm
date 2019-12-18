#include "Native.hpp"
#include "Python.hpp"
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
        assert(args != nullptr);
        int argNum = args->size();
        if(argNum != 1 || argNum != 3)
        {
            __throw_python_except("TypeError: type() takes 1 or 3 arguments\n");
        }
        // TODO 暂不考虑三个参数的情况
        PyObject *obj = args->get(0);
        return obj->klass()->type_object();
    }

    PyObject* isinstance(FuncArgs args)
    {
        assert(args != nullptr);
        if(args->size() != 2) 
            __throw_python_except("TypeError: isinstance expected 2 arguments, got %d\n", args->size());
        PyObject *arg1 = args->get(0);
        PyObject *arg2 = args->get(1); 
        if(!PyObject_Klass_Check0(arg2, TypeKlass))
        {
            __throw_python_except("TypeError: isinstance arg 2 must be a class, type, or tuple of classes and types\n");
        }
        PyTypeObject *typeObj = dynamic_cast<PyTypeObject*>(arg2);
        Klass *kls = arg1->klass();
        while (kls != nullptr)
        {
            if(kls->type_object() == typeObj) // equals to: kls == typeObj->own_klass()
                return VM::PyTrue;
            kls = kls->supper();
        }
        return VM::PyFalse;
    }
} // end of namespace python_builtins.


} // end of namespace native
