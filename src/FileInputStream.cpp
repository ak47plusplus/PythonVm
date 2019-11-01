#include "FileInputStream.hpp"
#include <memory> // for std::unique_ptr
#include <cstring>
#include <stdexcept>
#include <exception>
#include <cstdio>

FileInputStream::FileInputStream(const char *filename)
{
    m_in.open(filename, std::ios::in | std::ios::out);
    if(!m_in) {
        throw std::runtime_error("file not exist!");
    }
}

FileInputStream* FileInputStream::operator&()
{
    return std::addressof(*this);
}

void FileInputStream::close()
{
    if(m_in.is_open())
        m_in.close();
}

char FileInputStream::read()
{
    char c;
    m_in.get(c);
    return c;
}

void FileInputStream::unread()
{
    // TODO
}

int FileInputStream::read_int()
{
    char* arr = new char[4];
    memset(arr,0, sizeof(char)*4);
    std::shared_ptr<char> arr_ptr(arr, [](char *p) {delete[] p;});
    m_in.read(arr, 4);
    arr[0] = arr[0] & 0xff;
    arr[1] = arr[1] & 0xff;
    arr[2] = arr[2] & 0xff;
    arr[3] = arr[3] & 0xff;
    printf("%c %c %c %c\n",arr[0],arr[1],arr[2], arr[3]);
    return arr[3] << 24 | arr[2] << 16 | arr[1] << 8 | arr[0];
    // char a = this->read() & 0xff;
    // char b = this->read() & 0xff;
    // char c = this->read() & 0xff;
    // char d = this->read() & 0xff;
    // printf("%c %c %c %c\n",a,b,c,d );
    // return d << 24 | c << 16 | b << 8 | a;
}

FileInputStream::~FileInputStream()
{
    if (m_in.is_open())
        m_in.close();
}
