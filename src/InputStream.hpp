#ifndef INPUT_STREAM_HPP
#define INPUT_STREAM_HPP

#include <iostream>
#include <fstream>

#include "NonCopyable.hpp"

class InputStream : public fromboost::NonCopyable {
public:
    explicit InputStream(const char * filename);
    ~InputStream();
    InputStream* operator&();
    void close();
    char read();
    int read_int();
private:
    std::ifstream m_in;
};

#endif
