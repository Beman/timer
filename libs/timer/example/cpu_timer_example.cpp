//  run_timer_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/system/timer.hpp>
#include <cmath>

int main()
{
  boost::system::run_timer t;

  for ( long i = 0; i < 10000000; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
