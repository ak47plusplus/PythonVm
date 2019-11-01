#include "FileInputStream.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <exception>


FileInputStream::FileInputStream(const char *filename)
{
    m_In.open(filename, std::ios::in | std::ios::out);
    if(!m_In) {
        throw std::runtime_error("file not exist!");
    }
    //
    memset(std::addressof(this->m_LastBuffer, 0, BUFFER_SIZE));
    memset(std::addressof(this->m_Buffer, 0, BUFFER_SIZE));
    this->m_Index = 0;
    // read to buffer.
}

void FileInputStream::close()
{
    if(m_In.is_open())
        m_In.close();
}

char FileInputStream::read()
{
    if(m_Index < m_Max) {
        return m_Buffer[m_Index++];
    }
    else {
        // read from stream..... TODO
        return 'c';
    }
}

void FileInputStream::unread()
{
    m_Index--;
}

int FileInputStream::read_int()
{

    char a = this->read() & 0xff;
    char b = this->read() & 0xff;
    char c = this->read() & 0xff;
    char d = this->read() & 0xff;
    return d << 24 | c << 16 | b << 8 | a;
}

char FileInputStream::read_from_stream()
{
    char c;
    m_In.get(c);
    return c;
}
int FileInputStream::read_int_from_stream()
{
    char* arr = new char[4];
    memset(arr,0, sizeof(char)*4);
    std::shared_ptr<char> arr_ptr(arr, [](char *p) {delete[] p;});
    m_In.read(arr, 4);
    arr[0] = arr[0] & 0xff;
    arr[1] = arr[1] & 0xff;
    arr[2] = arr[2] & 0xff;
    arr[3] = arr[3] & 0xff;
    printf("%c %c %c %c\n",arr[0],arr[1],arr[2], arr[3]);
    return arr[3] << 24 | arr[2] << 16 | arr[1] << 8 | arr[0];
}


FileInputStream::~FileInputStream()
{
    if (m_In.is_open())
        m_In.close();
}
