#include "BinaryFileParser.hpp"
#include <cstdio> // meed to delete

BinaryFileParser::BinaryFileParser(FileInputStream *fileInputStream) NOEXCEPT
{
    this->m_FileStream = fileInputStream;
}

BinaryFileParser::~BinaryFileParser()
{
    // do nothing.
}

CodeObject* BinaryFileParser::parse()
{
    // 1. 读取魔数
    int magicNumber = m_FileStream->read_int();
    printf("magic nuber: %d\n", magicNumber);

    // 2. 读取更新时间
    int updateDate = m_FileStream->read_int();
    printf("update date: 0x%x\n", updateDate);

    // 3. object type
    char objectType = m_FileStream->read();
    if (objectType == 'c') {
        CodeObject* result = this->get_code_object();
        printf("Parser Ok@\n");
        return result;
    }
    return nullptr;
}

CodeObject *BinaryFileParser::get_code_object()
{
    return nullptr;
}
