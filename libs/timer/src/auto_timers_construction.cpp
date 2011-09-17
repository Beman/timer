//  boost auto_timers_construction.cpp  ------------------------------------------------//

//  Copyright Beman Dawes 2007, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/timer for documentation.

//--------------------------------------------------------------------------------------//

//  These constructors are in a separate file so that this translation unit will
//  not be linked in except when one of the constructors is actually used. This
//  is important since header <iostream> is required, and it incurs the cost of
//  the standard stream objects even if they are not used.

//--------------------------------------------------------------------------------------//

// define BOOST_TIMER_SOURCE so that <boost/timer/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_TIMER_SOURCE 

#include <boost/timer/timer.hpp>
#include <iostream>

namespace boost
{
  namespace timer
  {
    auto_cpu_timer::auto_cpu_timer(int places)
      : m_places(places), m_os(std::cout) {}

    auto_cpu_timer::auto_cpu_timer(const std::string& format, int places)
      : m_places(places), m_os(std::cout), m_format(format) {}

    auto_high_resolution_timer::auto_high_resolution_timer(int places)
      : m_places(places), m_os(std::cout) {}

    auto_high_resolution_timer::auto_high_resolution_timer(const std::string& format,
      int places)
      : m_places(places), m_os(std::cout), m_format(format) {}

  } // namespace timer
} // namespace boost
