#include "InputStream.hpp"

InputStream::InputStream(const char *filename)
{
    m_in.open(filename, std::ios::in | std:ios::out);
}

InputStream::~InputStream()
{
    if (m_in.is_open())
        m_in.close();
}
