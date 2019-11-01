#ifndef FILE_INPUT_STREAM_HPP
#define FILE_INPUT_STREAM_HPP

#include <iostream>
#include <fstream>

#include "NonCopyable.hpp"

/*
 * @brief A file stream with a buffer.
 * @Created by GuoXiang 2019/10/20
 */
class FileInputStream : public fromboost::NonCopyable {
public:
    explicit FileInputStream(const char * filename);
    ~FileInputStream();
    FileInputStream* operator&();
    void close();
    char read();
    void unread();
    int read_int();
private:
    std::ifstream m_in;
};

#endif
