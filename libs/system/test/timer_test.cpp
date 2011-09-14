//  boost timer_test.cpp  ----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#include <boost/system/timer.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::system::nanosecond_t;
using boost::system::times_t;
using boost::system::timer;
using boost::system::run_timer;

//----------------------------------------------------------------------------//
//
//  TODO
//
//  * Turn this program into a real test!
//  * Probe errors in format string handled properly
//
//----------------------------------------------------------------------------//

int main( int argc, char * argv[] )
{
  std::locale loc( "" );
  std::cout.imbue( loc );
  
  run_timer timer(6);
  run_timer timer2("\nwall %w s, utilization %p%\n");
  run_timer timer3("\nwall %w s, total cpu %t s, utilization %p%\n", 3);

  long count = 0;
  times_t times;
  times.clear();
  nanosecond_t timeout
    = nanosecond_t(100000000); // default .1 second

  if ( argc > 1 )
    timeout = nanosecond_t(std::atol( argv[1] ));

  while ( times.wall < timeout )
  {
    //  The point of this code is to burn both kernal and user cpu time,
    //  with the total less than wall clock time.
    ++count;
    timer.elapsed( times );
    std::cout << "iteration " << count << ", "
      << times.wall << " wall, "
      << times.user << " user, "
      << times.system << " system nanoseconds"
      << std::endl;
  }

  std::cout << count << " iterations\n";
  return 0;
}

