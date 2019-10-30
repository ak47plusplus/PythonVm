#ifndef FILE_INPUT_STREAM_HPP
#define FILE_INPUT_STREAM_HPP

#include <iostream>
#include <fstream>

#include "NonCopyable.hpp"

class FileInputStream : public fromboost::NonCopyable {
public:
    explicit FileInputStream(const char * filename);
    ~FileInputStream();
    FileInputStream* operator&();
    void close();
    char read();
    int read_int();
private:
    std::ifstream m_in;
};

#endif
