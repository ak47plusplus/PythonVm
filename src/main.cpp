#include <iostream>
#include <memory>
#include "FileInputStream.hpp"
#include "BinaryFileParser.hpp"

int main(int argc, char const *argv[]) {
    FileInputStream _stream("./test.txt");
    auto parser = std::make_shared<BinaryFileParser>(&_stream);
    parser->parse();
    return 0;
}
