#include "ArrayList.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"
#include "PyBool.hpp"

#include <stdio.h>

// 模板类 默认all放在hpp
// 这里存在特化版本 没办法....

/**
 * 针对PyObject*的模板特化版本. 使用equal比较而不是直接
 * 使用operator==比较.
 */
typedef PyObject *pPyObject;  // 直接用PyObject*就有问题...c++的编译器基本都是bug一堆
template<>
int ArrayList<pPyObject>::index(const pPyObject &t)
{
    if(m_size <= 0)
        return -1;
    for(auto i = 0; i < m_size; i++)
    {
        if(m_data[i]->equal(t) == VM::PyTrue)
            return i;
    }
    return -1;
}
