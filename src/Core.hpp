#ifndef CORE_HPP
#define CORE_HPP

#define VM_VERSION 2.7

#define DISABLE_COPY(_Ty) \
    private:  \
      _Ty(const _Ty&); \
      _Ty& operator=(const _Ty&);



#endif // endif CORE_HPP
