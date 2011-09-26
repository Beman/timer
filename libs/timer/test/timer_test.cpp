//  boost timer_test.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2006, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/timer for documentation.

#include <boost/timer/timer.hpp>
#include <boost/detail/lightweight_main.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;
using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::timer::format;
using boost::timer::cpu_timer;
using boost::timer::auto_cpu_timer;

namespace
{


  void format_test()
  {
    cout << "format test..." << endl;

    cpu_times times;
    times.wall = 5123456789LL;
    times.user = 2123456789LL;
    times.system = 1234567890LL;

    cout << "  times.wall is   " << times.wall << '\n';
    cout << "  times.user is   " << times.user << '\n';
    cout << "  times.system is " << times.system << '\n';
    cout << "  user+system is  " << times.user + times.system << '\n';
    cout << "  format(times, 9) output: " << format(times, 9);

    BOOST_TEST_EQ(format(times, 9),
      string(" 5.123456789s wall, 2.123456789s user + 1.234567890s system = 3.358024679s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 8),
      string(" 5.12345679s wall, 2.12345679s user + 1.23456789s system = 3.35802468s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 7),
      string(" 5.1234568s wall, 2.1234568s user + 1.2345679s system = 3.3580247s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 6),
      string(" 5.123457s wall, 2.123457s user + 1.234568s system = 3.358025s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 5),
      string(" 5.12346s wall, 2.12346s user + 1.23457s system = 3.35802s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 4),
      string(" 5.1235s wall, 2.1235s user + 1.2346s system = 3.3580s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 3),
      string(" 5.123s wall, 2.123s user + 1.235s system = 3.358s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 2),
      string(" 5.12s wall, 2.12s user + 1.23s system = 3.36s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 1),
      string(" 5.1s wall, 2.1s user + 1.2s system = 3.4s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 0),
      string(" 5s wall, 2s user + 1s system = 3s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, 10),
      string(" 5.123456789s wall, 2.123456789s user + 1.234567890s system = 3.358024679s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times, -1),
      string(" 5.123457s wall, 2.123457s user + 1.234568s system = 3.358025s cpu (65.5%)\n"));
    BOOST_TEST_EQ(format(times),
      string(" 5.123457s wall, 2.123457s user + 1.234568s system = 3.358025s cpu (65.5%)\n"));

    BOOST_TEST_EQ(format(times, 5, " %w, %u, %s, %t, %%p%"),
      string(" 5.12346, 2.12346, 1.23457, 3.35802, %65.5%"));

    BOOST_TEST_EQ(format(times, 5, "boo"), string("boo"));

    cout << "  format test complete" << endl; 
  }


}  // unnamed namespace

//--------------------------------------------------------------------------------------//

int cpp_main(int argc, char * argv[])
{
  cout << "----------  timer_test  ----------\n";
  format_test();
  return ::boost::report_errors();
}

