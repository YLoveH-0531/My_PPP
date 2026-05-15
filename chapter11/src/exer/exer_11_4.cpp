#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;
