#ifndef NONCIOYABLE_HPP
#define NONCIOYABLE_HPP

// like boost::noncopyable.
// disable copy constructor and operator=
class NonCopyable {
protected:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;

  NonCopyable() = default;
  ~NonCopyable() = default;
};

#endif
