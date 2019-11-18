#include "PyList.hpp"
#include "PyInteger.hpp"
#include "VM.hpp"
#include "Panic.hpp"

#include <assert.h>
#include <stdio.h>
#include <vector>

ListKlass* ListKlass::m_Instance = nullptr;
std::mutex ListKlass::m_Mutex;

ListKlass::ListKlass()
{}

ListKlass *ListKlass::get_instance()
{
    if (nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new ListKlass();
        }
    }
    return m_Instance;
}

/**
 * @brief 打印一个PyList.
 * 例如: [1,2,"3"]
 */
void ListKlass::print(PyObject * x)
{
    assert(x && x->klass() == this);
    PyList *pList = dynamic_cast<PyList*>(x);
    printf("[");
    for(auto i = 0; i < pList->size(); i++)
    {
        pList->get(i)->print();
        if(i < pList->size() -1)
        {
            printf(",");
        }
    }
    printf("]");
}

/**
 * @brief  对一个PyList进行取下标操作
 * @param lhs 需要取下标的list本身
 * @param rhs 下标值,必须是PyInteger
 */
PyObject *ListKlass::subscr(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == this);
    assert(rhs && rhs->klass() == IntegerKlass::get_instance());
    PyList * list = static_cast<PyList*>(lhs);
    PyInteger *index = static_cast<PyInteger*>(rhs);
    return list->get(index->value());
}

/**
 * @brief 判断当前list是否包含某个对象(==使用equals)
 * @param list本身
 * @param 被判断的对象
 */
PyObject *ListKlass::contains(PyObject *lhs, PyObject *rhs)
{
    assert(lhs && lhs->klass() == this);
    if(rhs == nullptr) return VM::PyFalse;
    PyList *list = dynamic_cast<PyList*>(lhs);
    if(list->size() == 0) return VM::PyFalse;
    return list->index(rhs) == -1 ? VM::PyFalse: VM::PyTrue;
}

/**
 * @brief 默认创建一个PyList.
 */
PyList::PyList()
{
    set_klass(ListKlass::get_instance());
    m_InnerList = new ArrayList<PyObject*>();
}

/**
 * @brief 根据一个已有的ArrayList<PyObject*>*创建一个PyList
 * PyList的内部list指针直接指向参数传递的list指针,参数传递完毕后
 * 请勿delete.这里并没有做复制.
 */
PyList::PyList(ArrayList<PyObject*> *list)
{
    set_klass(ListKlass::get_instance());
    m_InnerList = list;
}

/**
 * @brief 拷贝构造
 * 根据一个已有的PyList构造一个自己的list.
 * 对内部的ArrayList做复制.
 */
PyList::PyList(const PyList &rhs)
{
    set_klass(ListKlass::get_instance());
    this->m_InnerList = new ArrayList<PyObject*>(rhs.m_InnerList->size());
    for(auto i = 0; i < m_InnerList->size(); i++)
    {
        m_InnerList[i] = rhs.m_InnerList[i];
    }
}

/**
 * @brief 移动构造
 * 将一个PyList进行移动语义的内部替换.
 * 本方法完成后 原对象内部的ArrayList所有权转移到当前对象中.
 */
PyList::PyList(PyList &&rhs)
{
    set_klass(ListKlass::get_instance());
    this->m_InnerList = rhs.m_InnerList;
    rhs.m_InnerList = nullptr;
}

PyList& PyList::operator=(const PyList &rhs)
{
    if(m_InnerList != nullptr)
        delete m_InnerList;
    // made copy
    this->m_InnerList = new ArrayList<PyObject*>(rhs.m_InnerList->size());
    for(auto i = 0; i < m_InnerList->size(); i++)
    {
        m_InnerList[i] = rhs.m_InnerList[i];
    }
    return *this;
}

int PyList::count(PyObject *target)
{
    size_t cnt = 0;
    auto size = m_InnerList->size();
    for(auto i = 0; i < size; i ++)
    {
        if((*m_InnerList)[i]->equal(target))
            cnt++;
    }
    return cnt;
}

PyList::~PyList()
{
    if(m_InnerList != nullptr)
        delete m_InnerList;
}


namespace pylist {

/**
 * @brief 给list追加一个元素
 * @param args[0] list本身
 * @param args[1] 追加的元素
 */
PyObject* list_append(FuncArgs args)
{
    PyList *list = dynamic_cast<PyList*>(args->get(0));
    PyObject *new_element = args->get(1);
    list->append(new_element);
    return VM::PyNone;
}

/**
 * @brief 在list的指定位置插入一个元素
 * 如果插入位置的索引大于size，那么则视为追加。
 * <pre>
 * lst = []
 * lst.insert(100, 'ab')
 * print lst // ['ab'] , size = 1
 * </pre>
 * @param args[0] list本身
 * @param args[1] 插入位置的索引，必须为PyInteger
 * @param args[2] 插入的数据
 */
PyObject *list_insert(FuncArgs args)
{
    PyObject *arg0 = args->get(0);
    PyObject *arg1 = args->get(1);
    PyObject *arg2 = args->get(2);
    assert(arg0 && arg0->klass() == ListKlass::get_instance());
    if(arg1 == nullptr || arg1->klass() != IntegerKlass::get_instance())
    {
        __panic("'?' object cannot be interpreted as an integer\n");
    }
    PyList *list = dynamic_cast<PyList*>(arg0);
    PyInteger *index = dynamic_cast<PyInteger*>(arg1);
    if(index->value() > list->size()-1)
        list->append(arg2);
    else 
        list->insert(index->value(), arg2);
    return VM::PyNone;
}

/**
 * @brief 获取一个对象在list中的索引，不在list中则抛出异常
 */
PyObject *list_index(FuncArgs args)
{
    PyList* list = dynamic_cast<PyList*>(args->get(0));
    PyObject *target = args->get(1);
    for(auto i = 0; i < list->size(); ++i)
    {
        if(list->get(i)->equal(target))
        {
            return new PyInteger(i);
        }
    }
    __throw_python_except("ValueError: object not in list\n");
}

/**
 * @brief 对一个list进行pop操作
 * @param args[0] 操作的list
 */
PyObject *list_pop(FuncArgs args)
{
    PyList* list = dynamic_cast<PyList*>(args->get(0));
    if(list->size() <= 0)
        __throw_python_except("IndexError: pop from empty list\n");
    list->pop();
    return VM::PyNone;
}

/**
 * @brief 从list删除一个元素，
 * 如一个list中多次出现，那么只删除第一个.
 */
PyObject *list_remove(FuncArgs args)
{
    PyList* list = dynamic_cast<PyList*>(args->get(0));
    PyObject *target = args->get(1);
    auto index = -1;
    for(auto i = 0; i < list->size(); i++)
    {
        if(list->get(i)->equal(target))
        {
            index = i;
        }
    }
    if(index == -1)
        __throw_python_except("ValueError: list.remove(x): x not in list");
    list->delete_index(index);
    return VM::PyNone;
}

PyObject *list_clear(FuncArgs args)
{
    PyList* list = dynamic_cast<PyList*>(args->get(0));
    list->clear();
}

PyObject *list_count(FuncArgs args)
{
    PyList* list = dynamic_cast<PyList*>(args->get(0));
    PyObject *target = args->get(0);
    return new PyInteger(list->count(target));
}

} // end of namespace pylist
