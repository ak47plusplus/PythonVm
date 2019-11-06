#ifndef OS_H___
#define OS_H___

#include "Core.hpp"
#if (defined(NP_OS_LINUX)) && (NP_OS_LINUX)
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
typedef pid_t     PID;
typedef pthread_t TID;
#elif (defined(NP_OS_WINDOWS)) && (NP_OS_WINDOWS)
#include <TlHelp32.h>
#include <windows.h>
#include <WinBase.h>
#include <limits.h>
typedef DWORD     PID;
typedef DWORD     TID;
#else
#error "Unsupported Operating System."
#endif

inline PID GetCurrentProcessID()
{
#if (defined(NP_OS_LINUX)) && (NP_OS_LINUX)
    return getpid();
#else
    return GetCurrentProcessId();
#endif
}

inline PID GetParentProcessID()
{
#if (defined(NP_OS_LINUX)) && (NP_OS_LINUX)
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
#if (defined(NP_OS_LINUX)) && (NP_OS_LINUX)
    return syscall(SYS_gettid);
#else
    return GetCurrentThreadId();
#endif
}

#endif // end of OS_H__
