//  boost timer example.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994-2007, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/timer/timer.hpp>
#include <iostream>

using boost::timer::run_timer;
using std::cout;
using std::endl;

namespace
{
  void burn_time()
  {
    long long sum = 0, max = 5000000000;
    for (long long i = 0; i < max; ++i)
    {
      ++sum;
    }
    if (sum != max)
      throw("I don't believe this!");
  }
}

int main()
{
  {
    run_timer t;
    burn_time();
  }
  {
    run_timer t(4);
    burn_time();
  }
  {
    run_timer t(" %ws wall, %ts cpu (utilization %p%)\n");
    burn_time();
  }
  {
    run_timer t(" %ws wall, %ts cpu (utilization %p%)\n", 5);
    burn_time();
  }

  return 0;
}

//   Output (debug build):
//
//        11.04s wall, 11.04s user + 0.00s system = 11.04s cpu (100.0%)
//        11.0448s wall, 11.0449s user + 0.0000s system = 11.0449s cpu (100.0%)
//        11.01s wall, 11.01s cpu (utilization 100.0%)
//        11.04482s wall, 11.04487s cpu (utilization 100.0%)
