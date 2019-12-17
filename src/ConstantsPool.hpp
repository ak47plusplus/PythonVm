#ifndef CONSTANTS_POOL_HPP
#define CONSTANTS_POOL_HPP

#include <mutex>
#include "object/PyString.hpp"
#include "boost/noncopyable.hpp"

typedef class PyString *pPyString;

class ConstantsPool : public boost::noncopyable {
public:
    static ConstantsPool& Instance();
private:
    static ConstantsPool* m_Instance;
    static std::mutex     m_Mutex;
    ConstantsPool();
    ~ConstantsPool();
private:
    pPyString _MODULE_;
};

#endif