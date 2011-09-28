//  boost cpu_timer.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 1994-2006, 2011

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_TIMER_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_TIMER_SOURCE

#include <boost/timer/timer.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/system/system_error.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/throw_exception.hpp>
#include <boost/cerrno.hpp>
#include <cstring>
#include <cassert>

# if defined(BOOST_WINDOWS_API)
#   include <windows.h>
# elif defined(BOOST_POSIX_API)
#   include <unistd.h>
#   include <sys/times.h>
# else
# error unknown API
# endif

using boost::system::error_code;

namespace
{

# if defined(BOOST_POSIX_API)
  boost::int_least64_t tick_factor() // multiplier to convert ticks
                                     //  to nanoseconds; -1 if unknown
  {
    static boost::int_least64_t tick_factor = 0;
    if (!tick_factor)
    {
      if ((tick_factor = ::sysconf(_SC_CLK_TCK)) <= 0)
        tick_factor = -1;
      else
      {
        assert(tick_factor <= 1000000000LL); // logic doesn't handle large ticks
        tick_factor = 1000000000LL / tick_factor;  // compute factor
        if (!tick_factor)
          tick_factor = -1;
      }
    }
    return tick_factor;
  }
# endif

  void get_cpu_times(boost::timer::cpu_times& current)
  {
    boost::chrono::duration<boost::int64_t, boost::nano>
      x (boost::chrono::high_resolution_clock::now().time_since_epoch());
    current.wall = x.count();

# if defined(BOOST_WINDOWS_API)

    FILETIME creation, exit;
    if (::GetProcessTimes(::GetCurrentProcess(), &creation, &exit,
            (LPFILETIME)&current.system, (LPFILETIME)&current.user))
    {
      current.user   *= 100;  // Windows uses 100 nanosecond ticks
      current.system *= 100;
    }
    else
    {
      current.wall = current.system = current.user = boost::timer::nanosecond_type(-1);
    }
# else
    tms tm;
    clock_t c = ::times(&tm);
    if (c == -1) // error
    {
      current.system = current.user = boost::timer::nanosecond_type(-1);
    }
    else
    {
      current.system = boost::timer::nanosecond_type(tm.tms_stime + tm.tms_cstime);
      current.user = boost::timer::nanosecond_type(tm.tms_utime + tm.tms_cutime);
      if (tick_factor() != -1)
      {
        current.user *= tick_factor();
        current.system *= tick_factor();
      }
      else
      {
        current.user = current.system = boost::timer::nanosecond_type(-1);
      }
    }
# endif
  }

} // unnamed namespace

namespace boost
{
  namespace timer
  {

    BOOST_TIMER_DECL
    const std::string&  default_format()
    {
      static std::string fmt(" %ws wall, %us user + %ss system = %ts cpu (%p%)\n");
      return fmt;
    }

    //  cpu_timer  ---------------------------------------------------------------------//

    void cpu_timer::start()
    {
      m_is_stopped = false;
      get_cpu_times(m_times);
    }

    const cpu_times& cpu_timer::stop()
    {
      if (is_stopped())
        return m_times;
      m_is_stopped = true;
      
      cpu_times current;
      get_cpu_times(current);
      m_times.wall = (current.wall - m_times.wall);
      m_times.user = (current.user - m_times.user);
      m_times.system = (current.system - m_times.system);
      return m_times;
    }

    cpu_times cpu_timer::elapsed() const
    {
      if (is_stopped())
        return m_times;
      cpu_times current;
      get_cpu_times(current);
      current.wall -= m_times.wall;
      current.user -= m_times.user;
      current.system -= m_times.system;
      return current;
    }

    void cpu_timer::resume()
    {
      if (is_stopped())
      {
        cpu_times current (m_times);
        start();
        m_times.wall   -= current.wall;
        m_times.user   -= current.user;
        m_times.system -= current.system;
      }
    }
  } // namespace timer
} // namespace boost