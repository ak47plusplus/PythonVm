#include "Klass.hpp"
#include "Python.hpp"
#include "PyString.hpp"
#include "Panic.hpp"
#include "VM.hpp"
#include "PyBool.hpp"
#include "PyNoneType.hpp"d

#include <cassert>


PyObject* Klass::toBool(PyObject *x)
{
    return VM::PyFalse;
}

/**
 * 创建一个class 用于自定义类
 * @param locals class的CodeObject执行后保存的运行时栈帧的局部变量表。
 * @param supers 父类的tuple
 * @param classNameObj class的名称
 * @return class的TypeObject
 */
PyObject* Klass::create_klass(PyObject *locals, PyObject *supers, PyObject * classNameObj)
{
    assert(locals && PyObject_Klass_Check0(locals, DictKlass));
    assert(supers && supers->klass() == dynamic_cast<Klass*>(TupleKlass::get_instance()));
    assert(classNameObj && classNameObj->klass() == dynamic_cast<Klass*>(StringKlass::get_instance()));

    auto *classAttrs = dynamic_cast<PyDict*>(locals);
    auto *classSupers = dynamic_cast<PyTuple*>(supers);
    auto *className = dynamic_cast<PyString*>(classNameObj);

    auto *newClass = new Klass();
    // 设置类的成员属性和成员方法
    newClass->set_attrs(classAttrs);
    // 设置类的类名
    newClass->set_name(className->value());
    // 设置父类 暂时只支持单继承
    if(classSupers->size() > 0) {
        auto *theSuper = dynamic_cast<PyTypeObject*>(classSupers->get(0));
        newClass->set_super(theSuper->own_klass());
    }
    
	PyTypeObject *typeObj = new PyTypeObject();
	typeObj->set_own_klass(newClass);
    return typeObj;
}


PyObject* Klass::allocate_instance(PyObject *typeObj, Klass::FuncArgs args)
{
	PyObject *instance = new PyObject();
	PyTypeObject *typeObject = dynamic_cast<PyTypeObject*>(typeObj);
	instance->set_klass(typeObject->own_klass());
	return instance;
}
