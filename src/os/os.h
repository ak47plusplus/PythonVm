#ifndef OS_H___
#define OS_H___

#include "Core.hpp"
#if defined(NP_OS_LINUX) && (NP_OS_LINUX == 1)
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
typedef pid_t     PID;
typedef pthread_t TID;
#elif defined(NP_OS_WINDOWS) && (NP_OS_WINDOWS == 1)
#include <TlHelp32.h>
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

inline PID GetParentProcessID()
{
#if defined(NP_OS_LINUX) && (NP_OS_LINUX == 1)
    return getppid();
#else
    PID pid = -1 ;
    PID ppid = -1 ;
    HANDLE hSnapshot = 0 ;
    PROCESSENTRY32 pe = {0} ;
    pe.dwSize = sizeof(PROCESSENTRY32) ;
    hSnapshot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 ) ;
    if ( hSnapshot == 0 )
    goto error ;
    pid = GetCurrentProcessId () ;
    if ( Process32First ( hSnapshot, &pe ) )
    {
    do
    {
        if ( pe.th32ProcessID == pid )
        {
            ppid = pe.th32ParentProcessID ;
            goto done ;
        }
    } while ( Process32Next ( hSnapshot, &pe ) ) ;
    }
    done:
        return ppid ;
    error :
        goto done ;
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
