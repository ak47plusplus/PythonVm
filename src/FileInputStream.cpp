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
    if(m_Index < BUFFER_LEN ) {
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
    char a = read();
    char b = read();
    char c = read();
    char d = read();
	printf("readInt: a=%x b=%x c=%x d=%x \t", (char)a, (char)b, (char)c, (char)d);
    a = a & 0xff;
    b = b & 0xff;
    c = c & 0xff;
    d = d & 0xff;
    int ret = (d << 24 | c << 16 | b << 8 | a);
    printf("int = %d\n", ret);
    return ret;
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
