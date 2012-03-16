//
// To build:
//     $ clang++ -std=c++0x -stdlib=libc++ hello.cpp
//
// To run:
//     $ ./hello
//
#include <iostream>

int main( int argc, char* argv[] )
{
    unsigned i = 0;
    while ( i != argc )
    {
        std::cout << "argv[" << i << "]=" << argv[i] << std::endl;
        ++i;
    }
    return 0;
}


