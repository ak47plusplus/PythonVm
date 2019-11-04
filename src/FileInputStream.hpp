#ifndef FILE_INPUT_STREAM_HPP
#define FILE_INPUT_STREAM_HPP

#include <iostream>
#include <fstream>
#include "NonCopyable.hpp"

#define BUFFER_SIZE 512

/*
 * @brief A file stream with a buffer.
 * @Created by GuoXiang 2019/10/20
 */
class FileInputStream : public fromboost::NonCopyable {
public:
    explicit FileInputStream(const char * filename);
    ~FileInputStream();
    void close();
    char read(); // operate buffer
    void unread();
    int read_int();
    char read_from_stream();
    int read_int_from_stream();
private:
    char m_LastBuffer[BUFFER_SIZE];
    char m_Buffer[BUFFER_SIZE];
    unsigned int m_Index;
    unsigned int m_Max; // for when eof.
    std::ifstream m_In;
};

#endif
