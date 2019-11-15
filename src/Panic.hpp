#ifndef PANIC_HPP__
#define PANIC_HPP__

#include <stdio.h>

#define __panic(format, ...)                        \
      do {                                          \
          fprintf(stderr,format,##__VA_ARGS__);     \
          abort();                                  \
      } while(0)

// python的异常还没实现 暂时用一个abort代替
#define __throw_python_except(format, ...)                 \
      do {                                          \
          fprintf(stderr,format,##__VA_ARGS__);     \
          abort();                                  \
      } while(0)


#endif
