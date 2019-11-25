#include <memory>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <exception>

#include "VM.hpp"
#include "Logger.hpp"
#include "LoggerConfig.hpp"
#include "CodeObject.hpp"
#include "Interpreter.hpp"
#include "FileInputStream.hpp"
#include "PycFileParser.hpp"
#include "ArrayList.hpp"
#include "PyInteger.hpp"


INITIALIZE_EASYLOGGINGPP

void print_vm_logo()
{
    printf("_____         _    _                __      __ __  __ \n");
    printf("|  __ \       | |  | |               \ \    / /|  \/  |\n");
    printf("| |__) |_   _ | |_ | |__    ___   _ __\ \  / / | \  / |\n");
    printf("|  ___/| | | || __|| '_ \  / _ \ | '_ \\ \/ /  | |\/| |\n");
    printf("| |    | |_| || |_ | | | || (_) || | | |\  /   | |  | |\n");
    printf("|_|     \__, | \__||_| |_| \___/ |_| |_| \/    |_|  |_|\n");
    printf("         __/ |   \n");
    printf("        |___/            \n");
    printf("\n\n");
}

int main(int argc, char const *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    LoggerConfig::init();
    VM::init();

    FileInputStream binaryStream(argv[1]);
    auto parser = std::make_shared<PycFileParser>(&binaryStream);
    CodeObject *priCode = parser->parse();
    assert(priCode != nullptr);

    print_vm_logo();
    VM::run(priCode);
    delete priCode;
    return 0;
}
