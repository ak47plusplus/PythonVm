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
    int magicNumber = m_FileStream->read_int();
    printf("magic nuber: 0x%x\n", magicNumber);

    int updateDate = m_FileStream->read_int();
    printf("update date: 0x%x\n", updateDate);

    char objectType = m_FileStream->read();
    if (objectType == 'c') {
        CodeObject* result = this->get_code_object();
        printf("Parser Ok!\n");
        return result;
    }
    return nullptr;
}

CodeObject *BinaryFileParser::get_code_object()
{
    int argCount = m_FileStream->read_int();
    int nLocals = m_FileStream->read_int();
    int stackSize = m_FileStream->read_int();
    int flags = m_FileStream->read_int();

    return nullptr;
}
