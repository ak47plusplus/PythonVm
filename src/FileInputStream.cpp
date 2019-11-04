#include "FileInputStream.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <exception>


FileInputStream::FileInputStream(const char *filename)
{
    m_In.open(filename, std::ios_base::in| std::ios_base::binary);
    if(!m_In) {
        throw std::runtime_error("file not exist!");
    }
    m_Index = 0;
    m_In.read(m_Buffer, BUFFER_LEN);
}

void FileInputStream::close()
{
    if(m_In.is_open())
        m_In.close();
}

char FileInputStream::read()
{
    if(m_Index < BUFFER_LEN) {
        return m_Buffer[m_Index++];
    }
    else {
        m_Index = 0;
        m_In.read(m_Buffer, BUFFER_LEN);
        return m_Buffer[m_Index++];
    }
}

void FileInputStream::unread()
{
    m_Index--;
}

int FileInputStream::read_int()
{
    char a = read() & 0xff;
    char b = read() & 0xff;
    char c = read() & 0xff;
    char d = read() & 0xff;
	printf("readInt: a=%d b=%d c=%d d=%d \n", a, b, c, d);
    return d << 24 | c << 16 | b << 8 | a;
}

double FileInputStream::read_double()
{
     char arr[8];
     for(int i = 0; i < 8; i++)
     {
        arr[i] = read();
     }
     return *(double*)arr;
}

FileInputStream::~FileInputStream()
{
    if (m_In.is_open())
        m_In.close();
}
