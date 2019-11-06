#ifndef STACK_HPP_____
#define STACK_HPP_____

#include <stack>

template<typename T>
class Stack {
typedef unsigned int        size_type;
typedef T                   value_type;
typedef value_type&         reference;
typedef const value_type&   const_reference;
public:
    Stack() = default;
    size_type   size()                        { return m_InnerStack.size();}
    void        push(const value_type& val)   { m_InnerStack.push(val);}
    void        push(value_type&& val)        { m_InnerStack.push(val);}
    reference&  top();
private:
    std::stack<T> m_InnerStack;
};

#endif
