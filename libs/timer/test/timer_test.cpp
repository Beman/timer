//  boost timer_test.cpp  ----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/timer for documentation.

#include <boost/timer/timer.hpp>
#include <boost/detail/lightweight_main.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::timer::high_resolution_timer;
using boost::timer::auto_high_resolution_timer;
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

int cpp_main( int argc, char * argv[] )
{
  // These fail on Mac OS X if environment not previously set up
  //std::locale loc( "" );
  //std::cout.imbue( loc );

  long count = 0;
  
  {
    auto_cpu_timer timer(6);
    auto_cpu_timer timer2("\nwall %w s, utilization %p%\n");
    auto_cpu_timer timer3("\nwall %w s, total cpu %t s, utilization %p%\n", 3);

    cpu_times times;
    times.clear();
    nanosecond_type timeout
      = nanosecond_type(100000000); // default .1 second

    if ( argc > 1 )
      timeout = nanosecond_type(std::atol( argv[1] ));

    while ( times.wall < timeout )
    {
      //  The point of this code is to burn both kernal and user cpu time,
      //  with the total less than wall clock time.
      ++count;
      times = timer.elapsed();
      std::cout << "iteration " << count << ", "
        << times.wall << " wall, "
        << times.user << " user, "
        << times.system << " system nanoseconds"
        << std::endl;
    }
  }
  std::cout << count << " iterations\n\n";



  cpu_times start;
  start.clear();
  cpu_times now;

  {
    std::cout << "measure resolution - boost::timer::high_resolution_timer"
                 " resolution for wall-clock time..." << std::endl;

    auto_high_resolution_timer auto_hi_res(9);
    high_resolution_timer hi_res;

    for (int i = 0; i < 10; ++i)
    {
      now.wall = start.wall = hi_res.elapsed();
      while (now.wall == start.wall)
      {
        now.wall = hi_res.elapsed();
      }
      std::cout << now.wall - start.wall << "ns\n";
    }
  }

  cpu_timer cpu;

  {
    std::cout << "measure resolution - boost::timer::cpu_timer"
                 " resolution for wall-clock time..." << std::endl;
    auto_high_resolution_timer auto_hi_res(9);
    for (int i = 0; i < 10; ++i)
    {
      cpu.start();
      start = cpu.elapsed();
      now.wall = start.wall;
      while (now.wall == start.wall)
      {
        now = cpu.elapsed();
      }
      std::cout << now.wall - start.wall << "ns\n";
    }
  }

  cpu.start();
  start = cpu.elapsed();
  now.user = start.user;
  while (now.user == start.user)
  {
    now = cpu.elapsed();
  }
  std::cout << now.user - start.user
            << " ns measured resolution - boost::timer::cpu_timer resolution for user time\n";

  cpu.start();
  start = cpu.elapsed();
  now.system = start.system;
  while (now.system == start.system)
  {
    now = cpu.elapsed();
  }
  std::cout << now.system - start.system
            << " ns measured resolution - boost::timer::cpu_timer resolution for system time\n";

  return 0;
}

