//  boost auto_cpu_timer.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 1994-2006, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt)

//----------------------------------------------------------------------------//

// define BOOST_TIMER_SOURCE so that <boost/timer/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_TIMER_SOURCE 

#include <boost/timer/timer.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/assert.hpp>
#include <string>
#include <sstream>
#include <cstring>
#include <cassert>

using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::system::error_code;

# if defined(BOOST_WINDOWS_API)
#   include <windows.h>
# elif defined(BOOST_POSIX_API)
#   include <sys/times.h>
# else
# error unknown API
# endif

namespace
{
  //  cpu_timer helpers  ---------------------------------------------------------------//

  const char * default_format =
    " %ws wall, %us user + %ss system = %ts cpu (%p%)\n";

  void show_time(const cpu_times& times,
    std::ostream& os, const std::string& fmt, int places)
  //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
  //  be as low as 10, although will be 15 for many common platforms.
  {
    const char* format (fmt.empty() ? default_format : fmt.c_str());

    if (places > 9)
      places = 9;
    else if (places < 0)
      places = 0;

    boost::io::ios_flags_saver ifs(os);
    boost::io::ios_precision_saver ips(os);
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os.precision(places);

    const long double sec = 1000000000.0L;
    nanosecond_type total = times.system + times.user;
    long double wall_sec = times.wall / sec;
    long double total_sec = total / sec;

    for (; *format; ++format)
    {
      if (*format != '%' || !*(format+1) || !std::strchr("wustp", *(format+1)))
        os << *format;  // anything except % followed by a valid format character
                        // gets sent to the output stream
      else
      {
        ++format;
        switch (*format)
        {
        case 'w':
          os << times.wall / sec;
          break;
        case 'u':
          os << times.user / sec;
          break;
        case 's':
          os << times.system / sec;
          break;
        case 't':
          os << total / sec;
          break;
        case 'p':
          os.precision(1);
          if (wall_sec > 0.001L && total_sec > 0.001L)
            os << (total_sec/wall_sec) * 100.0;
          else
            os << "n/a";
          os.precision(places);
          break;
        default:
          assert(0);  // program logic error in "// anything except..." if stmt 
                      // above; no user error should ever fire this assert
        }
      }
    }
  }

  //  high_resolution_timer helpers  ---------------------------------------------------//

  const char* default_hi_res_format =
    " %ws elapsed wall clock time\n";

  void show_time(boost::timer::nanosecond_type time,
    std::ostream& os, const std::string& fmt, int places)
  //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
  //  be as low as 10, although will be 15 for many common platforms.
  {
    const char* format (fmt.empty() ? default_hi_res_format : fmt.c_str());

    if (places > 9)
      places = 9;
    else if (places < 0)
      places = 0;

    boost::io::ios_flags_saver ifs(os);
    boost::io::ios_precision_saver ips(os);
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os.precision(places);

    const long double sec = 1000000000.0L;

    for (; *format; ++format)
    {
      if (*format != '%' || !*(format+1) || !std::strchr("w", *(format+1)))
        os << *format;  // anything except % followed by a valid format character
                        // gets sent to the output stream
      else
      {
        ++format;
        switch (*format)
        {
        case 'w':
          os << time / sec;
          break;
        default:
          assert(0);  // program logic error in "// anything except..." if stmt 
                      // above; no user error should ever fire this assert
        }
      }
    }
  }

}  // unnamed namespace

namespace boost
{
  namespace timer
  {
    //  format  ------------------------------------------------------------------------//

    std::string format(nanosecond_type time, short places, const std::string& fmt)
    {
      std::stringstream ss;
      show_time(time, ss, fmt, places);
      return ss.str();
    }

    std::string format(const cpu_times& times, short places, const std::string& fmt)
    {
      std::stringstream ss;
      show_time(times, ss, fmt, places);
      return ss.str();
    }

    //  high_resolution_timer  ---------------------------------------------------------//

    void high_resolution_timer::report()
    {
      BOOST_ASSERT_MSG(m_os,
        "high_resolution_timer::report() requires automatic reporting");
      show_time(stop(), *m_os, m_format, m_places);
     }

    //  cpu_timer  ---------------------------------------------------------------------//

    void auto_cpu_timer::report()
    {
      show_time(stop(), m_os, m_format, m_places);
    }

  } // namespace timer
} // namespace boost
