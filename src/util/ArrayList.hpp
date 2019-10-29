#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

/**
 *  @brief A implementation of dynamic array like java.util.ArrayList
 *  @Created by GuoXiang 2019/10/29
 */
template<typename T>
class ArrayList {
public:
    explicit ArrayList(int initial_cap = 16);
    void add(T t);
    void insert(int index, T t);
    T get(int index);
    void set(int index, T t);
    int size();
    int capacity();
    T pop();
    bool isEmpty();
private:
    void expandCapacity();
private:
    T * m_data;
    int m_size;
    int m_capacity;
};

#endif
