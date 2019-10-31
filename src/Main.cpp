#include <iostream>
#include <memory>
#include "Logger.hpp"
#include "FileInputStream.hpp"
#include "BinaryFileParser.hpp"


INITIALIZE_EASYLOGGINGPP

/**
 * Main method and the boostrap.
 */
int main(int argc, char const *argv[]) {
    // init the easylogging++
    START_EASYLOGGINGPP(argc, argv);

    FileInputStream _stream("test.pyc");
    auto parser = std::make_shared<BinaryFileParser>(&_stream);
    parser->parse();
    return 0;
}
