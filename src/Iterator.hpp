#ifndef ITERATOR_HPP__
#define ITERATOR_HPP__ 1

template<typename T>
class Iterator {
public:
    Iterator();
    virtual ~Iterator();
    virtual bool has_next() = 0;
    virtual T    next()    = 0;
};

#endif