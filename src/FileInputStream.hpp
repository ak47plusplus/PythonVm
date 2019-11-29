#ifndef FILE_INPUT_STREAM_HPP
#define FILE_INPUT_STREAM_HPP

#include <iostream>
#include <fstream>

#include "boost/noncopyable.hpp"

#define BUFFER_LEN 256

/*
 * @brief A file stream with a buffer.
 * @Created by GuoXiang 2019/10/20
 */
class FileInputStream : public boost::noncopyable {
public:
    explicit FileInputStream(const char * filename);
    ~FileInputStream();

    void    close();
    char    read();
    void    unread();
    int     read_int();
    double  read_double();
private:
    char          m_Buffer[BUFFER_LEN];
    unsigned int  m_Index;
    std::ifstream m_In;
};

#endif
