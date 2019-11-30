//
// Created by guoxiang on 2019/11/30.
//

#ifndef INTERNALTHREAD_HPP
#define INTERNALTHREAD_HPP

#include <functional>

#include "Core.hpp"
#include "boost/thread.hpp"
#include "boost/noncopyable.hpp"

class Runnable {
public:
    virtual ~Runnable(){}
    virtual Run() = 0;
};

class InternalThread : public noncopyable {
    using tid = boost::thread::id;
public:
    explicit InternalThread(Runnable *target)
        : thread_(std::mem_fn(&Runnable::Run(), target))
    {

    }
    void Join()     { thread_.join(); }
    void Detach()   { thread_.detach(); }
    tid GetId() const NOEXCEPT    { return thread_.get_id(); }
    bool Joinable() const NOEXCEPT { return thread_.joinable(); }
private:
    boost::thread thread_;
};

#endif //TINYPYTHON_INTERNALTHREAD_H
