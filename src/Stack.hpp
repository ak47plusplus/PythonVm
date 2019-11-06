#ifndef STACK_HPP_____
#define STACK_HPP_____

#include <stack>

template<typename T>
class Stack {
public:
    Stack() = default;
private:
    std::stack<T> m_InnerStack;
};

#endif
