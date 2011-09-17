//  boost/timer/timer.hpp  -------------------------------------------------------------//

//  Copyright Beman Dawes 1994-2007, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_TIMER_TIMER_HPP                  
#define BOOST_TIMER_TIMER_HPP

#include <boost/config/warning_disable.hpp>

#include <boost/timer/config.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <cstring>
#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#   if defined(_MSC_VER)
#     pragma warning(push)           // Save warning settings
#     pragma warning(disable : 4251) // disable warning: class 'std::basic_string<_Elem,_Traits,_Ax>'
#   endif                            // needs to have dll-interface...

//--------------------------------------------------------------------------------------//

//  TODO:
//  
//  * Add BOOST_NOEXCEPT where applicable

//--------------------------------------------------------------------------------------//

namespace boost
{
  namespace timer
  {
    typedef boost::int_least64_t nanosecond_t;

    struct times_t
    {
      nanosecond_t wall;
      nanosecond_t user;
      nanosecond_t system;

      void clear() { wall = user = system = 0LL; }
    };

    class high_resolution_timer;
    class auto_high_resolution_timer;
    class cpu_timer;
    class auto_cpu_timer;

    //  high_resolution_timer  ---------------------------------------------------------//

    class BOOST_TIMER_DECL high_resolution_timer
    {
    public:

      high_resolution_timer()                       : m_flags(m_stopped)
                                                    { start(); }
      high_resolution_timer(const std::nothrow_t&)
                                                    : m_flags(static_cast<m_flags_t>(
                                                        m_stopped | m_nothrow))
                                                    { start(); }
     ~high_resolution_timer()                       {}

      void            start()
      {
        m_flags = static_cast<m_flags_t>(m_flags& ~m_stopped);
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        m_time = now.count();
      }
      nanosecond_t    stop()
      {
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        m_time = now.count() - m_time;
        m_flags = static_cast<m_flags_t>(m_flags | m_stopped);
        return m_time;
      }
      bool            stopped() const               { return m_flags & m_stopped; }
      nanosecond_t    elapsed()                     // does not stop()
      {
        if (stopped())
          return m_time;
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        return now.count() - m_time;
      }
    private:
      nanosecond_t    m_time;
      enum            m_flags_t                     { m_stopped=1, m_nothrow=2 };
      m_flags_t       m_flags;
    };

    //  auto_high_resolution_timer  ----------------------------------------------------//

    class BOOST_TIMER_DECL auto_high_resolution_timer : public high_resolution_timer
    {
    public:

      // Each constructor has two overloads to avoid an explicit default to std::cout.
      // Such a default would require including <iostream>, with its high costs, even
      // when the standard streams are not used.

      explicit auto_high_resolution_timer(int places = 3);

      auto_high_resolution_timer(int places, std::ostream& os)  : m_places(places),
                                                      m_os(os) {}

      explicit auto_high_resolution_timer(const std::string& format, int places = 3);

      auto_high_resolution_timer(const std::string& format, int places, std::ostream& os)
                                               : m_places(places), m_os(os),
                                                 m_format(format) {}

     ~auto_high_resolution_timer()
      { 
        if(!stopped())
          report();
      }

      void            report();

    private:
      int             m_places;
      std::ostream&   m_os;
      std::string     m_format;  
    };

    //  cpu_timer  ---------------------------------------------------------------------//

    class BOOST_TIMER_DECL cpu_timer
    {
    public:

      cpu_timer()                                   : m_flags(m_stopped) { start(); }
      cpu_timer(const std::nothrow_t&)              : m_flags(static_cast<m_flags_t>(
                                                        m_stopped | m_nothrow))
                                                    { start(); }
     ~cpu_timer()                                   {}  // never throws

      void            start();
      const times_t&  stop();
      bool            stopped() const               { return m_flags& m_stopped; }
      void            elapsed(times_t& result);     // does not stop()

    private:
      times_t         m_times;
      enum            m_flags_t                     { m_stopped=1, m_nothrow=2 };
      m_flags_t       m_flags;
    };

    //  auto_cpu_timer  ----------------------------------------------------------------//

    class BOOST_TIMER_DECL auto_cpu_timer : public cpu_timer
    {
    public:

      // Each constructor has two overloads to avoid an explicit default to std::cout.
      // Such a default would require including <iostream>, with its high costs, even
      // when the standard streams are not used.

      explicit auto_cpu_timer(int places = 3);

      auto_cpu_timer(int places, std::ostream& os)  : m_places(places),
                                                      m_os(os) {}

      explicit auto_cpu_timer(const std::string& format, int places = 3);

      auto_cpu_timer(const std::string& format, int places, std::ostream& os)
                                               : m_places(places), m_os(os),
                                                 m_format(format) {}

     ~auto_cpu_timer()
      { 
        if(!stopped())
          report();
      }

      void            report();

    private:
      int             m_places;
      std::ostream&   m_os;
      std::string     m_format;  
    };
    
  } // namespace timer
} // namespace boost

#   if defined(_MSC_VER)
#     pragma warning(pop) // restore warning settings.
#   endif 

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_TIMER_TIMER_HPP
