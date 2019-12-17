//
// Created by guoxiang on 2019/11/30.
//
#include <cassert>
#include <iostream>
#include <string>

#include "VM.hpp"
#include "ConstantsPool.hpp"
#include "PyTypeObject.hpp"
#include "PyDict.hpp"

std::mutex TypeKlass::m_Mutex;
TypeKlass* TypeKlass::m_Instance = nullptr;

TypeKlass::TypeKlass()
{
    set_name("type");
    set_attrs(new PyDict());
    // TypeKlass的typeObject咋办 TODO
}

TypeKlass *TypeKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new TypeKlass();
        }
    }
    return m_Instance;
}

TypeKlass::print(PyObject *obj)
{
    assert(obj && obj->klass() == this);
    std::cout << "<type ";
    PyTypeObject *typeObject = dynamic_cast<PyTypeObject*>(obj);
    Klass *ownKlass = typeObject->own_klass();
    PyDict* attrDict = ownKlass->attrs();
    assert(attrDict != nullptr);
    PyObject *module = attrDict->get(static_cast<PyObject*>(ConstantsPool::Instance()._MODULE_));
    if(module != VM::PyNone)
    {
        module->print();
        std::cout << ".";
    }
    std::cout << ownKlass->name();
    std::cout << ">";
}

PyTypeObject::PyTypeObject()
{
    set_klass(TypeKlass::get_instance());
    // TODO.
}

void PyTypeObject::set_own_klass(Klass* klass)
{
    m_OwnKlass = klass;
    klass->set_type_object(this);
}

Klass* PyTypeObject::own_klass()
{
    return m_OwnKlass;
}
