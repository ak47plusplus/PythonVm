//
// Created by guoxiang on 2019/11/30.
//

#ifndef INTERNALTHREAD_HPP
#define INTERNALTHREAD_HPP

#include <functional>

#include "Core.hpp"
#include "boost/thread.hpp"
#include "boost/noncopyable.hpp"

/*
 * 这里之所以使用boost::thread而不是std::thread更不是底层的pthread，是因为
 * 为了以后实现python线程时需要各自的解释器对象所以使用了boost::thread_specific_ptr
 * 而boost::thread_specific_ptr再与非boost线程搭配使用时可能会导致线程结束而线程本地
 * 对象无法被正确删除的风险.
 */

class Runnable {
public:
    virtual ~Runnable(){}
    virtual void Run() = 0;
};

class InternalThread : public noncopyable {
    using tid = boost::thread::id;
public:
    explicit InternalThread(Runnable *target)
        : thread_(&Runnable::Run(), target){}
    void Join()     { thread_.join(); }
    void Detach()   { thread_.detach(); }
    tid GetId() const NOEXCEPT    { return thread_.get_id(); }
    bool Joinable() const NOEXCEPT { return thread_.joinable(); }
private:
    boost::thread thread_;
};

#endif //TINYPYTHON_INTERNALTHREAD_H
