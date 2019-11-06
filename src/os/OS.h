#ifndef OS_HPP___
#define OS_HPP___

#include "Core.hpp"

#if defined(NP_OS_LINUX) && (NP_OS_LINUX == 1)
    typedef pid_t     PID;
    typedef pthread_t TID;
#elif defined(NP_OS_WINDOWS) && (NP_OS_WINDOWS == 1)
    typedef DWORD     PID;
    typedef DWORD     TID;
#else
    #error "Unsupported os system"
#endif

inline PID GetCurrentProcessID()
{
#if defined(NP_OS_LINUX) && (NP_OS_LINUX == 1)
    return getpid();
#else
    return GetCurrentProcessId();
#endif
}

inline TID GetCurrentThreadID()
{
#if defined(NP_OS_LINUX) && (NP_OS_LINUX == 1)
    return syscall(SYS_gettid);
#else
    return GetCurrentThreadId();
#endif
}



#endif // endof OS_HPP__
