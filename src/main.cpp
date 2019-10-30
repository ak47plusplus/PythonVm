#include <iostream>
#include <memory>
#include "FileInputStream.hpp"
#include "BinaryFileParser.hpp"

int main(int argc, char const *argv[]) {
  FileInputStream _stream("./test.txt");
  // 1 ok
  std::shared_ptr<BinaryFileParser> parser(new BinaryFileParser(&_stream));
  parser->parse();
  // 2 error g++ -o app *.cpp -std=c++11
  // auto parser = std::make_shared<BinaryFileParser>(new BinaryFileParser(&_stream));
  // parser->parse();
  return 0;
}
