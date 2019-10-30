#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "Core.hpp"
#include "CodeObject.hpp"
#include "NonCopyable.hpp"
#include "FileInputStream.hpp"


class BinaryFileParser : public fromboost::NonCopyable {
public:
    explicit BinaryFileParser(FileInputStream *FileInputStream) NOEXCEPT;
    ~BinaryFileParser();
    CodeObject* parse();
private:
    FileInputStream* m_FileStream;
};

#endif
