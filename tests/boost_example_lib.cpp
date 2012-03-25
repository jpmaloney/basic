// Copyright 2012 John P. Maloney
//
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)
//
// To build:
//     $ g++ -I ~/thirdparty/boost 
//         boost_example_lib.cpp -o boost_example_lib 
//         ~/thirdparty/boost/stage/lib/libboost_regex.a
//
// To run:
//     $ ./boost_example_lib
//
// Reference:
// http://www.boost.org/doc/libs/1_49_0/more/getting_started/unix-variants.html#id24
//
#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main()
{
    std::string line;
    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }
}

