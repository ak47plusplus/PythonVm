#ifndef PY_INTEGER_HPP
#define PY_INTEGER_HPP

/**
 *  @brief The Python Integer.
 *  @Created by GuoXiang 2019/10/29
 */
class PyInteger : public PyObject {
public:
  PyInteger(int x) : m_value(x){}
  int value() {return this->m_value;}
private:
  int m_value;
};

#endif
