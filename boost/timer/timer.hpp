//  boost/timer/timer.hpp  -------------------------------------------------------------//

//  Copyright Beman Dawes 1994-2007, 2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_TIMER_TIMER_HPP                  
#define BOOST_TIMER_TIMER_HPP

#include <boost/config/warning_disable.hpp>

#include <boost/timer/config.hpp>
#include <boost/chrono/chrono.hpp>
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

class high_resolution_timer;
class auto_high_resolution_timer;
class cpu_timer;
class auto_cpu_timer;

typedef boost::int_least64_t nanosecond_type;

struct cpu_times
{
  nanosecond_type wall;
  nanosecond_type user;
  nanosecond_type system;

  void clear() { wall = user = system = 0LL; }
};
      
static const std::string   default_format;
static const short         default_places = 6;

std::string  format(nanosecond_type time,
                    short places = default_places,
                    const std::string& fmt = default_format);

std::string format(const cpu_times& times,
                   short places = default_places,
                   const std::string& fmt = default_format); 

//  high_resolution_timer  -------------------------------------------------------------//
//
//  provides the same resolution but lower latency than using cpu_timer for
//  wall-clock timings
//
//  all functions are noexcept unless otherwise specified 

class BOOST_TIMER_DECL high_resolution_timer
{
public:

  //  constructors, destructor
  high_resolution_timer()                : m_os(0) { start(); }

  //  these 4 constructors imply automatic reporting on destruction
  explicit high_resolution_timer(std::ostream& os,
                                 short places = default_places,
                                 const std::string& format = default_format)
                                 : m_places(places), m_os(&os), m_format(format)
                                 { start(); }
  high_resolution_timer(std::ostream& os, const std::string& format)
                                 : m_places(default_places), m_os(&os), m_format(format)
                                 { start(); }
  explicit high_resolution_timer(short places, const std::string& format = default_format);
  explicit high_resolution_timer(const std::string& format);

 ~high_resolution_timer();

  //  observers
  bool             is_stopped() const    { return m_is_stopped; }
  nanosecond_type  elapsed() const; // does not stop()
  std::string      format(const std::string& fmt = default_format,
                            int places = default_places) const
                                         { return timer::format(elapsed(), places, fmt); }
  //  actions
  void             start();
  nanosecond_type  stop();
  void             resume();
  void             report();  // requires: automatic reporting

private:
  nanosecond_type  m_time;
  bool             m_is_stopped;

  //  These variables implement automatic reporting on destruction and are not otherwise
  //  used. They could be replaced by a pointer to an implementation object, and it could
  //  be zero when automatic reporting is not active. The space savings doesn't appear
  //  worth the trouble.
  short            m_places;
  std::ostream*    m_os;        // 0 unless automatic reporting is active
  std::string      m_format;
};

////  auto_high_resolution_timer  --------------------------------------------------------//
////
////  all functions are noexcept unless otherwise specified 
//
//class BOOST_TIMER_DECL auto_high_resolution_timer : public high_resolution_timer
//{
//public:
//
//  // Each constructor has two overloads to avoid an explicit default to std::cout.
//  // Such a default would require including <iostream>, with its high costs, even
//  // when the standard streams are not used.
//
//  explicit
//  auto_high_resolution_timer(int places = default_places);
//
//  auto_high_resolution_timer(int places, std::ostream& os) : m_places(places),
//                                                             m_os(os) {}
//  explicit
//  auto_high_resolution_timer(const std::string& format, int places = default_places);
//    // may throw
//  auto_high_resolution_timer(const std::string& format, int places, std::ostream& os)
//          : m_places(places), m_os(os), m_format(format) {}
//    // may throw
//
//  std::ostream& report();  // calls stop(), may throw
//
//  ~auto_high_resolution_timer()
//  { 
//    if (!is_stopped())
//    {
//      try
//      {
//        report();
//      }
//      catch (...) // eat any exceptions
//      {
//      }
//    }
//  }
//
//private:
//  int             m_places;
//  std::ostream&   m_os;
//  std::string     m_format;  
//};

//  cpu_timer  ---------------------------------------------------------------------//
//
//  all functions are noexcept unless otherwise specified 

class BOOST_TIMER_DECL cpu_timer
{
public:

  //  constructors, destructor
  cpu_timer()                                    { start(); }
 ~cpu_timer()                                    {}

  //  observers
  bool              is_stopped() const           { return m_is_stopped; }
  cpu_times         elapsed() const;  // does not stop()
  std::string       format(const std::string& fmt = default_format,
                              int places = default_places) const
                                            { return timer::format(elapsed(), places, fmt); }
  //  actions
  void              start();
  const cpu_times&  stop();
  void              resume(); 

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

  explicit
  auto_cpu_timer(int places = default_places);

  auto_cpu_timer(int places, std::ostream& os)  : m_places(places),
                                                  m_os(os) {}
  explicit
  auto_cpu_timer(const std::string& format, int places = default_places);   // may throw
       
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

  void   report();

private:
  int             m_places;
  std::ostream&   m_os;
  std::string     m_format;  
};

//  implementation  ----------------------------------------------------------------//
 
  inline
  void high_resolution_timer::start()
  {
    m_is_stopped = false;
    boost::chrono::duration<boost::int_least64_t, boost::nano> now
      (boost::chrono::high_resolution_clock::now().time_since_epoch());
    m_time = now.count();
  }
 
  inline
  nanosecond_type high_resolution_timer::stop()
  {
    boost::chrono::duration<boost::int_least64_t, boost::nano> now
      (boost::chrono::high_resolution_clock::now().time_since_epoch());
    m_time = now.count() - m_time;
    m_is_stopped = true;
    return m_time;
  }
 
  inline
  nanosecond_type high_resolution_timer::elapsed() const // does not stop()
  {
    if (is_stopped())
      return m_time;
    boost::chrono::duration<boost::int_least64_t, boost::nano> now
      (boost::chrono::high_resolution_clock::now().time_since_epoch());
    return now.count() - m_time;
  }

  inline
  void high_resolution_timer::resume()
  {
    if (is_stopped())
    {
      nanosecond_type current (m_time);
      start();
      m_time -= current;
    }
  }
   
  } // namespace timer
} // namespace boost

#   if defined(_MSC_VER)
#     pragma warning(pop) // restore warning settings.
#   endif 

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_TIMER_TIMER_HPP
