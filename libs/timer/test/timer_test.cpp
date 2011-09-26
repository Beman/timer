//  boost cpu_timer_test.cpp  ----------------------------------------------------------//

//  Copyright Beman Dawes 2006, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/timer for documentation.

#include <boost/timer/timer.hpp>
#include <boost/detail/lightweight_main.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::timer::cpu_timer;
using boost::timer::auto_cpu_timer;

//--------------------------------------------------------------------------------------//

int cpp_main( int argc, char * argv[] )
{

  return ::boost::report_errors();
}

