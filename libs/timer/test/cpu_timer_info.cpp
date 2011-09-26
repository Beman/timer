//  boost cpu_timer_info.cpp  ----------------------------------------------------------//

//  Copyright Beman Dawes 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/timer for documentation.

#include <boost/timer/timer.hpp>
#include <boost/detail/lightweight_main.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::timer::cpu_timer;
using boost::timer::auto_cpu_timer;


int cpp_main( int argc, char * argv[] )
{
  cpu_times start_time;
  start_time.clear();
  cpu_times current_time;

  {
    cpu_timer cpu;
    std::cout << "measure boost::timer::cpu_timer resolution for user time..."
              << std::endl;
    for (int i = 0; i < 3; ++i)
    {
      cpu.start();
      start_time = cpu.elapsed();
      current_time.user = start_time.user;
      while (current_time.user == start_time.user)
      {
        current_time = cpu.elapsed();
      }
      std::cout << current_time.user - start_time.user << "ns\n";
    }
  }
 
  {
    cpu_timer cpu;
    std::cout << "measure boost::timer::cpu_timer resolution for wall-clock time..."
              << std::endl;
    for (int i = 0; i < 1000; ++i)
    {
      cpu.start();
      start_time.wall = cpu.elapsed().wall;
      current_time.wall = start_time.wall;
      while (current_time.wall == start_time.wall)
      {
        current_time.wall = cpu.elapsed().wall;
      }
      std::cout << current_time.wall - start_time.wall << ' ';
    }
  }
 return 0;
}

