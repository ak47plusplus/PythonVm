#include "BinaryFileParser.hpp"

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
  return nullptr;
}
