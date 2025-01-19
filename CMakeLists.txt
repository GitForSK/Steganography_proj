cmake_minimum_required(VERSION 3.20)
project(projekt_steganografia)

set(CMAKE_CXX_STANDARD 17)

add_executable(projekt_steganografia main.cpp BMP_Coder/BMP_Class.hpp TGA_Coder/TGA_Class.hpp Steganography_methods/Methods_file.hpp Steganography_methods/Console_comands.hpp Steganography_methods/crypt_methods.hpp)
