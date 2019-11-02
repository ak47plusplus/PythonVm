#ifndef PANIC_HPP__
#define PANIC_HPP__

#include <stdio.h>
#define __panic(format, ...)            \
      do {                        \
          fprintf(stderr,format,__VA_ARGS__);  \
          abort();                \
      } while(0)

#endif
