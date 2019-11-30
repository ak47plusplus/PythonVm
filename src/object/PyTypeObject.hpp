//
// Created by mzygd on 2019/11/30.
//

#ifndef PYTYPEOBJECT_HPP
#define PYTYPEOBJECT_HPP

#include <mutex>

#include "Klass.hpp"

class TypeKlass : public Klass {
public:
    static TypeKlass *get_instance();
private:
    TypeKlass();
    static TypeKlass *m_Instance;
    static std::mutex m_Mutex;
};



#endif //PYTYPEOBJECT_HPP
