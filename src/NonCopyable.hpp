#ifndef NONCIOYABLE_HPP
#define NONCIOYABLE_HPP

#include "Core.hpp"

FROM_BOOST_BEGIN

// like boost::noncopyable.
// disable copy constructor and operator=
class NonCopyable {
protected:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;

  NonCopyable() = default;
  ~NonCopyable() = default;
};


FROM_BOOST_END // endof namespace fromboost

#endif
