
#define OS_ARCH "amd64"

#define COMPARE_AND_SWAP_64(addr, old_val, new_val) \
({                                                  \
    char result;                                    \
    __asm__ __volatile__ ("                         \
        lock;                                       \
        cmpxchgq %4, %1;                            \
        sete %0"                                    \
    : "=q" (result), "=m" (*addr)                   \
    : "m" (*addr), "a" ((uintptr_t)old_val),        \
      "r" ((uintptr_t)new_val)                      \
    : "memory");                                    \
    result;                                         \
})

#define COMPARE_AND_SWAP_32(addr, old_val, new_val) \
({                                                  \
    char result;                                    \
    __asm__ __volatile__ ("                         \
        lock;                                       \
        cmpxchgl %4, %1;                            \
        sete %0"                                    \
    : "=q" (result), "=m" (*addr)                   \
    : "m" (*addr), "a" (old_val), "r" (new_val)     \
    : "memory");                                    \
    result;                                         \
})
