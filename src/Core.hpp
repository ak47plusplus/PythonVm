#ifndef CORE_HPP
#define CORE_HPP

/*
 * The defination of python vm version.
 */
#define VM_VERSION 2.7

/*
 * Succedaneum of throw() in cpp11 and higher cplusplus version.
 */
#define NOEXCEPT noexcept

/*
 * A macron of disable move constructor and operator=
 */
#define DISABLE_COPY(_Ty) \
    private:  \
      _Ty(const _Ty&); \
      _Ty& operator=(const _Ty&);

/*
 * 3rd namespace.
 */
#define FROM_BOOST_BEGIN namespace fromboost {
#define FROM_BOOST_END }



#endif // endif CORE_HPP
