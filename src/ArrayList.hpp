#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <cstdio>
#include <exception>
#include <stdexcept>

#include "VM.hpp"

/**
 *  @brief A implementation of dynamic array like java.util.ArrayList
 *  @Created by GuoXiang 2019/10/29
 */
template<typename T>
class ArrayList {
public:
    explicit ArrayList(int initial_cap = 16);
    ArrayList(const ArrayList& rhs);
    ~ArrayList();
    ArrayList& operator=(const ArrayList& rhs);
    T& operator[](int index);// Returns a Reference to the element at position n in the vector container.
    void add(T t);
    void insert(int index, T t);
    T get(int index);
    void set(int index, T t);
    int index(const T &t);
    int size();
    int capacity();
    T pop();
    T top();
    bool isEmpty();
private:
    void expandCapacity();
private:
    T * m_data;
    int m_size;
    int m_capacity;
};

template<typename T>
ArrayList<T>::ArrayList(int initial_cap)
{
    if (initial_cap <= 0) {
      initial_cap = 16;
    }
    this->m_capacity = initial_cap;
    this->m_size = 0;
    this->m_data = new T[initial_cap];
}

// Copy constructor implementation.
template<typename T>
ArrayList<T>::ArrayList(const ArrayList& rhs)
{
    if(m_data != nullptr)
        delete[] m_data;
    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;
    m_data = new T[m_capacity];
    for (size_t i = 0; i < m_capacity; i++) {
        m_data[i] = rhs.m_data[i];
    }
}

// Operator= implementation.
template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs)
{
    if(m_data != nullptr)
        delete[] m_data;
    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;
    m_data = new T[m_capacity];
    for (size_t i = 0; i < m_capacity; i++) {
        m_data[i] = rhs.m_data[i];
    }
    return *this;
}

// Returns a Reference instead of a Copy to the element at position index in the ArrayList container.
// so we can have the usage like: list[0] = 3
// like std::vector, the operator[] wont do the range check.
template<typename T>
T& ArrayList<T>::operator[](int index)
{
    return m_data[index];
}

template<typename T>
void ArrayList<T>::add(T t)
{
    if(this->m_size >= m_capacity) {
        this->expandCapacity();
    }
    this->m_data[m_size++] = t;
}

template<typename T>
void ArrayList<T>::insert(int index, T t)
{
    this->add(t); // 只为了添加一个占位, 添加的内容会被覆盖.
    for(int i = this->m_size-1; i > index; i--)
    {
      m_data[i] = m_data[i-1];
    }
    m_data[index] = t;
}

// Range checked
template<typename T>
T ArrayList<T>::get(int index)
{
  if (index >= m_size) {
      throw std::out_of_range("index out of bounds.");
  }
  return m_data[index];
}

// TODO
template<typename T>
void ArrayList<T>::set(int index, T t)
{
    while(index >= m_capacity-1)
        this->expandCapacity();

    this->m_data[index] = t;
    if(index >= m_size) // 5/8
        m_size = index + 1;
}

/**
 * 泛化版本的index
 */
template<typename T>
int ArrayList<T>::index(const T &t)
{
    if(m_size <= 0)
        return -1;
    for(auto i = 0; i < m_size; i++)
    {
        if(m_data[i] == t)
            return i;
    }
    return -1;
}


template<typename T>
void ArrayList<T>::expandCapacity()
{
    T *new_space = new T[m_capacity << 1]; // capacity expands to 2 times.
    for (size_t i = 0; i < m_capacity; i++) {
        new_space[i] = m_data[i];
    }
    delete[] m_data;
    m_data = new_space;
    m_capacity <<= 1;
}

template<typename T>
int ArrayList<T>::size()
{
    return this->m_size;
}

template<typename T>
int ArrayList<T>::capacity()
{
   return this->m_capacity;
}

template<typename T>
T ArrayList<T>::pop()
{
    if(m_size <= 0)
    {
        throw std::out_of_range("empty list and pop failed.");
    }
    return m_data[--m_size];
}

template<typename T>
T ArrayList<T>::top()
{
    if(m_size <= 0)
    {
        throw std::out_of_range("empty list and top failed.");
    }
    return m_data[m_size-1];
}

template<typename T>
bool ArrayList<T>::isEmpty()
{
    return this->m_size == 0;
}

template<typename T>
ArrayList<T>::~ArrayList()
{
    if (m_data != nullptr) {
      delete[] m_data;
    }
}

#endif
