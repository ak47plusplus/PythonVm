//
// Created by guoxiang on 2019/11/30.
//

#ifndef PYTHREAD_H
#define PYTHREAD_H

#include "Interpreter.hpp"
#include "InternalThread.hpp"

// TODO
//  每一个python线程映射成一个posixThread
// 每个thread需要一个interpreter.

class PyThreadRunnable {
public:
    explicit PyThreadRunnable()
    {
        this->inter = Interpreter::Get();
    }
private:
    Interpreter & thr_interpreter_;
};

#endif //TINYPYTHON_PYTHREAD_H
