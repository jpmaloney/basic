// Copyright 2012 John P. Maloney
//
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "extension.hpp"

using std::array;
using std::cout;
using std::endl;
using std::vector;

namespace
{

void usage()
{
    cout << "Usage: extension [integer ...]\n"
            "Output for each integer: integer+1, 2*integer\n"
         << endl;
}

} // End local namespace.


namespace example1
{

auto const desc = "No substitution";

inline int f_increment( int i ) { return i + 1; }

inline int f_double( int i ) { return i + i; }

void test_f_increment( int i )
{
    cout << "example1: f_increment( " << i << " ) -> " << f_increment( i ) << endl;
}

void test_f_double( int i )
{
    cout << "example1: f_double( " << i << " ) -> " << f_double( i ) << endl;
}

void test( int i )
{
    test_f_increment( i );
    test_f_double( i );
}

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        test( value );
}

} // End namespace example1.


namespace example2
{

auto const desc = "Run-time substitution via pointer-to-function";

namespace library
{

    typedef int (*func_t)( int );

    void test( func_t f, int i )
    {
        cout << "example2: fx( " << i << " ) -> " << f( i ) << endl;
    }

} // End namespace library.

namespace application
{

    inline int f_increment( int i ) { return i + 1; }

    inline int f_double( int i ) { return i + i; }

    void test( int i )
    {
        library::test( f_increment, i );
        library::test( f_double, i );
    }

} // End namespace application.

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        application::test( value );
}

} // End namespace example2.


namespace example3
{

auto const desc = "Run-time substitution via virtual function";

namespace library
{

    class abstract_f
    {
    public:
        virtual ~abstract_f() {}
        virtual int operator()( int ) const = 0;
    };

    void test( abstract_f const& f, int i )
    {
        cout << "example3: fx( " << i << " ) -> " << f( i ) << endl;
    }

} // End namespace library.

namespace application
{

    class f_increment : public library::abstract_f
    {
    public:
        virtual int operator()( int i ) const
        {
            return i + 1;
        }
    };

    class f_double : public library::abstract_f
    {
    public:
        virtual int operator()( int i ) const
        {
            return i + i;
        }
    };

    void test( int i )
    {
        library::test( f_increment(), i );
        library::test( f_double(), i );
    }

} // End namespace application.

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        application::test( value );
}

} // End namespace example3.


namespace example4
{

auto const desc = "Compile-time substitution via function-as-template-parameter";

namespace library
{

    template<typename Func>
    void test( Func f, int i )
    {
        cout << "example4: fx( " << i << " ) -> " << f( i ) << endl;
    }

} // End namespace library.

namespace application
{

    inline int f_increment( int i ) { return i + 1; }

    inline int f_double( int i ) { return i + i; }

    void test( int i )
    {
        library::test( f_increment, i );
        library::test( f_double, i );
    }

} // End namespace application.

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        application::test( value );
}

} // End namespace example4.


namespace example5
{

auto const desc = "Compile-time substitution via function object";

namespace library
{

    template<typename Func>
    void test( Func f, int i )
    {
        cout << "example5: fx( " << i << " ) -> " << f( i ) << endl;
    }

} // End namespace library.

namespace application
{

    class f_increment
    {
    public:
        int operator()( int i ) const { return i + 1; }
    };

    class f_double
    {
    public:
        int operator()( int i ) const { return i + i; }
    };

    void test( int i )
    {
        library::test( f_increment(), i );
        library::test( f_double(), i );
    }

} // End namespace application.

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        application::test( value );
}

} // End namespace example5.

namespace example6
{

auto const desc = "Compile-time substitution via function object";

namespace library
{

    template<typename Func>
    void test( Func f, int i )
    {
        cout << "example6: fx( " << i << " ) -> " << f( i ) << endl;
    }

} // End namespace library.

namespace application
{

    class f_increment
    {
    public:
        int operator()( int i ) const { return i + 1; }
    };

    class f_double
    {
    public:
        int operator()( int i ) const { return i + i; }
    };

    void test( int i )
    {
        library::test( f_increment(), i );
        library::test( f_double(), i );
    }

} // End namespace application.

template<typename Sequence>
void main( Sequence s )
{
    for ( auto const value : s )
        application::test( value );
}

} // End namespace example6.


namespace
{

template<typename Func, typename Sequence, typename Desc>
void test( Func f, Sequence s, Desc d )
{
    auto const line = "===========================================";
    cout << line << endl;
    cout << d << endl;
    f( s );
    cout << line << endl;
}

vector<int> parse( int argc, char* argv[] )
{
    vector<int> result;

    if ( argc <= 1 )
    {
        result = { 1, 2 };
    }
    else
    {
        for ( auto i = 1; i != argc; ++i )
            result.push_back( atoi( argv[i] ) );
    }

    return result;
}

} // End local namespace.

int main( int argc, char* argv[] )
{
    if ( argc <= 1 ) usage();

    auto const values = parse( argc, argv );

    test( example1::main<decltype(values)>, values, example1::desc );
    test( example2::main<decltype(values)>, values, example2::desc );
    test( example3::main<decltype(values)>, values, example3::desc );
    test( example4::main<decltype(values)>, values, example4::desc );
    test( example5::main<decltype(values)>, values, example5::desc );
    test( example6::main<decltype(values)>, values, example6::desc );

    return 0;
}


