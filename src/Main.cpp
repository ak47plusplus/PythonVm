#include <iostream>
#include <memory>
#include <stdlib.h>
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

    try {
        FileInputStream _stream("test.pyc");
        auto parser = std::make_shared<BinaryFileParser>(&_stream);
        parser->parse();
    } catch(std::exception &ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
