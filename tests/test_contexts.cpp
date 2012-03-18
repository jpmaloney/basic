// test_contexts.cpp
//
// References:
// http://linux.die.net/man/3/makecontext
// http://linux.die.net/man/2/setcontext
// http://linux.die.net/man/2/sigaltstack
// http://linux.die.net/man/3/strerror
//
// Copyright 2012 John P. Maloney
// 
// Distributed under the Apache License, Version 2.0.
// (See accompanying file LICENSE_2_0 or copy at
// http://www.apache.org/licenses/LICENSE-2.0)
//
#include <string.h>
#include <ucontext.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{

std::string string_from_errno( int e )
{
    enum { kBufSize = 128U };
    char buf[ kBufSize ];
    // 'strerror_r' is the thread-safe version of 'strerror'.
    // Assuming GNU implementation, which returns a char*.
    char const* value = strerror_r( e, buf, sizeof( buf ) );
    return std::string( value );
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
void swap_context( ucontext_t& current, ucontext_t next )
{
    auto const code = swapcontext( &current, &next );
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
    return os << "<todo>";
}

std::ostream& operator<<( std::ostream& os, mcontext_t const& rhs )
{
    return os << "<todo>";
}

std::ostream& operator<<( std::ostream& os, ucontext_t const& rhs )
{
    os << "("
       << "uc_link="      << std::hex << (void*)rhs.uc_link
       << " uc_sigmask="  << std::hex << rhs.uc_sigmask
       << " uc_stack="    << std::hex << rhs.uc_stack
       << " uc_mcontext=" << std::hex << rhs.uc_mcontext
       << ")"
       ;
    return os;
}

} // End of local namespace.

int main( int argc, char* argv[] )
{
    try
    {
        ucontext_t ctx = get_context();
        std::cout << "ucontext_t:\n" << ctx << std::endl;
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

