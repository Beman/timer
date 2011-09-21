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
    typedef boost::int_least64_t nanosecond_type;

    struct cpu_times
    {
      nanosecond_type wall;
      nanosecond_type user;
      nanosecond_type system;

      void clear() { wall = user = system = 0LL; }
    };

    class high_resolution_timer;
    class auto_high_resolution_timer;
    class cpu_timer;
    class auto_cpu_timer;

    //  high_resolution_timer  ---------------------------------------------------------//
    //
    //  provides the same resolution but lower latency than using cpu_timer for
    //  wall-clock timings
    //
    //  all functions are noexcept unless otherwise specified 

    class BOOST_TIMER_DECL high_resolution_timer
    {
    public:
      
      static const std::string   default_format;
      static const int           default_places = 6;

      high_resolution_timer()                { start(); }
     ~high_resolution_timer()                {}

      void             start()
      {
        m_is_stopped = false;
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        m_time = now.count();
      }
      nanosecond_type  stop()
      {
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        m_time = now.count() - m_time;
        m_is_stopped = true;
        return m_time;
      }
      bool             is_stopped() const    { return m_is_stopped; }
      nanosecond_type  elapsed() const // does not stop()
      {
        if (is_stopped())
          return m_time;
        boost::chrono::duration<boost::int_least64_t, boost::nano> now
          (boost::chrono::high_resolution_clock::now().time_since_epoch());
        return now.count() - m_time;
      }

      static
      std::string format(nanosecond_type time, const std::string& fmt = default_format,
                           int places = default_places);

      std::string format(const std::string& fmt = default_format,
        int places = default_places) const
      {
        return format(elapsed(), fmt, places);
      }
    private:
      nanosecond_type  m_time;
      bool             m_is_stopped;
    };

    //  auto_high_resolution_timer  ----------------------------------------------------//
    //
    //  all functions are noexcept unless otherwise specified 

    class BOOST_TIMER_DECL auto_high_resolution_timer : public high_resolution_timer
    {
    public:

      // Each constructor has two overloads to avoid an explicit default to std::cout.
      // Such a default would require including <iostream>, with its high costs, even
      // when the standard streams are not used.

      explicit auto_high_resolution_timer(int places = 6);

      auto_high_resolution_timer(int places, std::ostream& os)  : m_places(places),
                                                      m_os(os) {}

      explicit auto_high_resolution_timer(const std::string& format, int places = 6);
        // may throw
      auto_high_resolution_timer(const std::string& format, int places, std::ostream& os)
             : m_places(places), m_os(os), m_format(format) {}
        // may throw

      void report();  // calls stop(), may throw

     ~auto_high_resolution_timer()
      { 
        if (!is_stopped())
        {
          try
          {
            report();
          }
          catch (...) // eat any exceptions
          {
          }
        }
      }

    private:
      int             m_places;
      std::ostream&   m_os;
      std::string     m_format;  
    };

    //  cpu_timer  ---------------------------------------------------------------------//
    //
    //  all functions are noexcept unless otherwise specified 

    class BOOST_TIMER_DECL cpu_timer
    {
    public:

      cpu_timer()                                    { start(); }
     ~cpu_timer()                                    {}

      void              start();
      const cpu_times&  stop();
      bool              is_stopped() const           { return m_is_stopped; }
      cpu_times         elapsed() const;  // does not stop()

    private:
      cpu_times         m_times;
      bool              m_is_stopped;
    };

    //  auto_cpu_timer  ----------------------------------------------------------------//
    //
    //  all functions are noexcept unless otherwise specified 

    class BOOST_TIMER_DECL auto_cpu_timer : public cpu_timer
    {
    public:

      // Each constructor has two overloads to avoid an explicit default to std::cout.
      // Such a default would require including <iostream>, with its high costs, even
      // when the standard streams are not used.

      explicit auto_cpu_timer(int places = 3);

      auto_cpu_timer(int places, std::ostream& os)  : m_places(places),
                                                      m_os(os) {}

      explicit auto_cpu_timer(const std::string& format, int places = 3);   // may throw
       
      auto_cpu_timer(const std::string& format, int places, std::ostream& os) // may throw
                                               : m_places(places), m_os(os),
                                                 m_format(format) {}

     ~auto_cpu_timer()
      { 
        if (!is_stopped())
        {
          try
          {
            report();
          }
          catch (...) // eat any exceptions
          {
          }
        }
      }

      void            report();    // throws iff I/O throws

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
