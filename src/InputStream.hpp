#ifndef INPUT_STREAM_HPP
#define INPUT_STREAM_HPP

#include <iostream>
#include <fstream>

#include "NonCopyable.hpp"

class InputStream : public fromboost::NonCopyable {
public:
    InputStream(const char * filename);
    ~InputStream();
private:
    std::ifstream m_in;
};


#endif
