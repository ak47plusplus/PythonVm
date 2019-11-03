#include "Klass.hpp"
#include "PyString.hpp"

Klass::Klass()
{
    this->m_Name = nullptr;

}

Klass::~Klass()
{
    if(m_Name != nullptr) delete m_Name;
}
