//  boost timer_test.cpp  ----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/timer for documentation.

#include <boost/timer/timer.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::timer::nanosecond_t;
using boost::timer::times_t;
using boost::timer::cpu_timer;
using boost::timer::auto_cpu_timer;

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

  long count = 0;
  
  {
    auto_cpu_timer timer(6);
    auto_cpu_timer timer2("\nwall %w s, utilization %p%\n");
    auto_cpu_timer timer3("\nwall %w s, total cpu %t s, utilization %p%\n", 3);

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
  }
  std::cout << count << " iterations\n\n";

  count = 0;
  times_t start;
  times_t now;

  cpu_timer cpu;

  cpu.elapsed(start);
  now.wall = start.wall;
  while (now.wall == start.wall)
  {
    cpu.elapsed(now);
  }
  std::cout << now.wall - start.wall
            << "ns is the measured boost::timer::times() resolution for wall time\n";

  cpu.start();
  cpu.elapsed(start);
  now.user = start.user;
  while (now.user == start.user)
  {
    cpu.elapsed(now);
  }
  std::cout << now.user - start.user
            << "ns is the measured boost::timer::times() resolution for user time\n";

  cpu.start();
  cpu.elapsed(start);
  now.system = start.system;
  while (now.system == start.system)
  {
    cpu.elapsed(now);
  }
  std::cout << now.system - start.system
            << "ns is the measured boost::timer::times() resolution for system time\n";

  return 0;
}

