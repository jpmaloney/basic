// test_contexts.cpp
//
// References:
// http://linux.die.net/man/3/makecontext
// http://linux.die.net/man/2/setcontext
// http://linux.die.net/man/2/sigaltstack
// http://linux.die.net/man/3/errno
// http://linux.die.net/man/3/strerror_r
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/signal.h.html
//
// Copyright 2012 John P. Maloney
// 
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)
//
#include <ucontext.h>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::dec;
using std::endl;
using std::hex;

namespace
{

std::string string_from_errno( int e )
{
    enum { kBufSize = 256U }; // TODO: Choose this value more wisely.
    char buf[ kBufSize ];
    // 'strerror_r' is the thread-safe version of 'strerror'.

#   if defined(__FreeBSD__) \
    || (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE
    int value = strerror_r( e, buf, sizeof( buf ) );
    if ( value == 0 )
    {
        return buf;
    }
    else              
    {
        // If strerror_r fails, it sets 'errno'.  This seems like a race
        // condition, defeating the thread-safety of strerror_R,
        // but we'll try our best to interpret it.
        value = strerror_r( errno, buf, sizeof( buf ) );
        if ( value == 0 ) return buf;
        else              return "unknown errno";
    }
#   else // GNU-specific.
    char const* value = strerror_r( e, buf, sizeof( buf ) );
    return value; // 'value' may not be 'buf'.
#   endif
}

} // End local namespace.

namespace
{

// get_context - Acquire the current context.
ucontext_t get_context()
{
    ucontext_t result;
    auto const code = getcontext( &result );
    if ( code != 0 )
    {
        throw std::runtime_error( string_from_errno( errno ) );
    }
    return result;
}

// set_context - Go to 'context'.
void set_context( ucontext_t const& context )
{
    // A successful call to setcontext never returns.
    setcontext( &context );
    throw std::runtime_error( string_from_errno( errno ) );
}

// make_context - Returns an updated version of 'context'.
// TODO: How to map C++11 variadic templates to ANSI C var-args?
ucontext_t make_context( ucontext_t context, void (*func)(), int argc /* , ... */ )
{
    // makecontext updates 'context'.
    makecontext( &context, func, argc /* , ... */ );
    return context;
}

// swap_context - Atomically update 'current' and go to 'next'.
// A successful call to swapcontext does not return.
void swap_context( ucontext_t& result, ucontext_t next )
{
    auto const code = swapcontext( &result, &next );
    if ( code != 0 )
    {
        throw std::runtime_error( string_from_errno( errno ) );
    }
}

} // End of local namespace.

namespace
{

std::ostream& operator<<( std::ostream& os, sigset_t const& rhs )
{
    return os << "<todo>";
}

std::ostream& operator<<( std::ostream& os, stack_t const& rhs )
{
    os << "("
       << "ss_sp="     << ((void*)rhs.ss_sp)
       << " ss_size="  << rhs.ss_size
       << " ss_flags=" << rhs.ss_flags
       << ")";
    return os;
}

std::ostream& operator<<( std::ostream& os, mcontext_t const& rhs )
{
    return os << "<todo>";
}

std::ostream& operator<<( std::ostream& os, ucontext_t const& rhs )
{
    os << "("
       << "uc_link="      << hex << ((void*)rhs.uc_link) << dec
       << " uc_sigmask="  << rhs.uc_sigmask
       << " uc_stack="    << rhs.uc_stack
       << " uc_mcontext=" << rhs.uc_mcontext
       << ")"
       ;
    return os;
}

} // End of local namespace.

namespace test1
{

// Reference: This example taken from:
// http://linux.die.net/man/3/makecontext

ucontext_t uctx_main, uctx_func1, uctx_func2;

extern "C" void func1()
{
    cout << "func1: enter" << endl;
    cout << "func1: swap_context( uctx_func1, uctx_func2 )" << endl;
    swap_context( uctx_func1, uctx_func2 );
    cout << "func1: exit" << endl;
}

extern "C" void func2()
{
    cout << "func2: enter" << endl;
    cout << "func2: swap_context( uctx_func2, uctx_func1 )" << endl;
    swap_context( uctx_func2, uctx_func1 );
    cout << "func2: exit" << endl;
}

void test( int argc, char* argv[] )
{
    cout << "test1::test: enter" << endl;

    // Note: Stack size of 2048 is too low for x86-64 and clang++ 3.1.
    enum { kStackSize = 8192U }; // 4096U };
    char stack_func1[ kStackSize ];
    char stack_func2[ kStackSize ];

    uctx_func1 = get_context();
    uctx_func1.uc_stack.ss_sp = stack_func1;
    uctx_func1.uc_stack.ss_size = sizeof( stack_func1 );
    uctx_func1.uc_link = &uctx_main;
    uctx_func1 = make_context( uctx_func1, func1, 0 );

    uctx_func2 = get_context();
    uctx_func2.uc_stack.ss_sp = stack_func2;
    uctx_func2.uc_stack.ss_size = sizeof( stack_func2 );
    uctx_func2.uc_link = ( argc > 1 ) ? 0 : &uctx_func1;
    uctx_func2 = make_context( uctx_func2, func2, 0 );

    cout << "test1::test: swap_context( uctx_main, uctx_func2 )" << endl;
    swap_context( uctx_main, uctx_func2 );
    cout << "test1::test: exit" << endl; // FIXME: We don't get here on PC-BSD 9.
}

} // End namespace 'test1'.

int main( int argc, char* argv[] )
{
    try
    {
        test1::test( argc, argv );
    }
    catch ( std::exception const& e )
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    catch ( ... )
    {
        std::cerr << "Unknown exception" << std::endl;
        return 2;
    }
    return 0;
}

