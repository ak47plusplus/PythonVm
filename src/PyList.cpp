#include "PyList.hpp"

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
            m_Instance = nullptr;
        }
    }
    return m_Instance;
}

void ListKlass::print(PyObject *x)
{
    // TODO
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
}

PyList::~PyList()
{
    if(m_InnerList != nullptr)
        delete m_InnerList;
}