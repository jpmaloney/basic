// Copyright 2012 John P. Maloney
//
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)
//
// To build:
//   $ cd ~/basic/tests
//   $ g++ -I ~/thirdparty/boost boost_example.cpp -o boost_example
//
// To run:
//   $ echo 1 2 3 | ./boost_example
//
// Reference:
// http://www.boost.org/doc/libs/1_49_0/more/getting_started/unix-variants.html
//
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}

