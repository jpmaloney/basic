// Copyright 2012 John P. Maloney
// 
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)
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


