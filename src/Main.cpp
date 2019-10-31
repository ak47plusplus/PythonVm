#include <iostream>
#include <memory>
#include "FileInputStream.hpp"
#include "BinaryFileParser.hpp"

/**
 * Main method and the boostrap.
 */
int main(int argc, char const *argv[]) {
    FileInputStream _stream("./test.pyc");
    auto parser = std::make_shared<BinaryFileParser>(&_stream);
    parser->parse();
    return 0;
}
