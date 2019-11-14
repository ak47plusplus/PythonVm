#include "ArrayList.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include <stdio.h>

// 模板类 默认all放在hpp
// 这里存在特化版本 没办法....

/**
 * 针对PyObject*的模板特化版本. 使用equal比较而不是直接
 * 使用operator==比较.
 */
template<>
int ArrayList<PyObject *>::index(PyObject * &t)
{
    printf("version special for PyObject* \n");
    if(m_size <= 0)
        return -1;
    for(auto i = 0; i < m_size; i++)
    {
        if(m_data[i]->equal(t) == VM::PyTrue)
            return i;
    }
    return -1;
}

class ArrayList<PyObject*>;