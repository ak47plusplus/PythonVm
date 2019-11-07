#ifndef BLOCK_HPP__
#define BLOCK_HPP__

class Block {
public:
    uint8_t    m_Type;      // block的类型 除了wile还有except还有finnally等
    uint32_t   m_Target;    // 跳转的目标地址
    int32_t    m_Level;     // 进入block时 操作数栈的深度 对while block影响不大.

    Block()
    {
        m_Type = 0;
        m_Target = 0;
        m_Level = 0;
    }

    Block(uint8_t _type, uint32_t _target, int32_t _level)
        :m_Type(_type),m_Target(_target),m_Level(_level){}

    Block(const Block &rhs)
    {
        m_Type = rhs.m_Type;
        m_Target = rhs.m_Target;
        m_Level = rhs.m_Level;
    }
};


#endif
