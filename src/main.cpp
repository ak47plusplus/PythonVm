#include <iostream>
#include "FileInputStream.hpp"

int main(int argc, char const *argv[]) {
  FileInputStream is("./test.txt");
  std::cout << is.read_int() << std::endl;
  return 0;
}
