#include "InputStream.hpp"
#include <memory> // for std::unique_ptr

InputStream::InputStream(const char *filename)
{
    m_in.open(filename, std::ios::in | std::ios::out);
}

int InputStream::read_int()
{
    char* arr = new char[4];
    std::unique_ptr<char> arr_ptr(arr, [](char *p) {delete[] p;});
    m_in.read(arr, 4);
    arr[0] = arr[0] & 0xff;
    arr[1] = arr[1] & 0xff;
    arr[2] = arr[2] & 0xff;
    arr[3] = arr[3] & 0xff;
    return arr[3] << 24 | arr[2] << 16 | arr[1] << 8 | arr[0];
}

InputStream::~InputStream()
{
    if (m_in.is_open())
        m_in.close();
}
