//!
//!@file    xhelpers/sx_timestamp.h
//!@author  Sholomov Dmitry
//!@date    11.12.2017
//!@brief   Types for operating with global time with milliseconds precision
//!

#ifndef AR_TIMESTAMP_H
#define AR_TIMESTAMP_H

#pragma once

#include <string>
#include <ios>
#include <ctime>
#include <chrono>
#include <cassert>
#include <cstring>
#include <cmath>

#include <system.h>

namespace ar
{

//! @class timestamp_t
//! @brief Struct timestamp_t is to be defined as timespec or via timespec
//! @todo  move struct definition to the proper header
typedef struct timestamp_s
{
  time_t tv_sec;  //!< Seconds - >= 0
  long   tv_nsec; //!< Nanoseconds - [0, 999999999]
} timestamp_t;

//! @class timestamp
//! @brief Class timestamp implements base functions to work with timestamp_t structure
class timestamp : public timestamp_t
{
public:
  // Constructors and destructors
  timestamp();                                          //!< Default constructor, initialized by current time with milliseconds (not nanoseconds)
  timestamp(time_t tv_sec, long tv_nsec);               //!< Constructor from numeric values
  timestamp(const char* sTime);                         //!< Constructor from const char* representation in UTC format
  timestamp(const std::string& sTime);                  //!< Constructor from std::string representation in UTC format
  timestamp(const tm& tmTime, int nsec = 0);            //!< Constructor from tm structure
  timestamp(                                            //!< Constructor from numeric data fields
    int year, int mon, int day,                         //!<   date: years since 1900, months since January [0,11], day of month [1,31]
    int hour = 0, int min = 0, int sec = 0,             //!<   time (optional)
    int nsec = 0,                                       //!<   nanoseconds (optional)
    const std::string& sBias = "");                     //!<   proper bias for non-local time zone, e.g. "+03:00". Local time zone is taken if sBias is empty
  timestamp(int precision);                             //!< Constructor, initialized with current time with nanoseconds. Precision is a number 
                                                        //!< of digits in fraction of a second (e.g 0 - sec, 3 - ms, 9 - ns)

  bool isNull() const;                                  //!< true if timestamp is null
  bool isNegative() const;                              //!< true if timestamp is negative
  static const timestamp& null();                       //!< return null value 

  std::string toString() const;                         //!< Generate std::string for the time in UTC format
  std::string toString(int precision) const;            //!< Generate std::string for the time in UTC format. Precision is a number 
                                                        //!< of digits in fraction of a second
  timestamp& fromString(const std::string& sTime);      //!< Generate timestamp from std::string representation in UTC format

  // Casting operators
  operator std::string() const;                         //!< Generate std::string for the time in UTC format
  operator tm() const;                                  //!< Converting tv_sec to tm structure, tv_nsec is ommitted

  // Getting values
  int year() const;                                     //!< get years since 1900
  int mon() const;                                      //!< get months since January - [0,11]
  int day() const;                                      //!< get day of the month - [1,31]
  int hour() const;                                     //!< get hours since midnight - [0,23]
  int min() const;                                      //!< get minutes after the hour - [0,59]
  int sec() const;                                      //!< get seconds after the minute - [0,59]

  double msec() const;                                  //!< get milliseconds
  double usec() const;                                  //!< get microseconds
  long nsec() const;                                    //!< get nanoseconds

  // Setting values
  timestamp& set_msec(double msec);                     //!< set milliseconds
  timestamp& set_usec(double usec);                     //!< set microseconds
  timestamp& set_nsec(long nsec);                       //!< set nanoseconds
  timestamp& set_bias(const std::string& sBias);        //!< set non-local time bias and recalculate time value to local

  // Arithmetics
  timestamp& operator+=(const timestamp& dtime);        //!< Add time delta to the current time value
  timestamp& operator-=(const timestamp& dtime);        //!< Subtract time delta from the current time value
  timestamp& operator+=(double dsec);                   //!< Add time delta (in double seconds) to the current time value
  timestamp& operator-=(double dsec);                   //!< Subtract time delta  (in double seconds) from the current time value

  // Stream serialization
  friend std::istream& operator>>(std::istream& str, ar::timestamp& ts);       //!< Read myself from stream
  friend std::ostream& operator<<(std::ostream& str, const ar::timestamp& ts); //!< Write myself to stream

  // Static functions
  static timestamp diff(const timestamp& start, const timestamp& stop);         //!< Calculate time difference interval
  static std::string timeZone();                                                //!< Local time zone in string format, e.g. "+03:00"
  static long bias(const std::string& sTimeZone = "");                          //!< Offset from greenwich time in minutes, e.g. "+03:00" -> -180
  static const long maxNano();                                                  //!< returns nanoseconds in a single second

};

////////////////////////////////////////////////////////
//////  Class timestamp implementation section

inline timestamp::timestamp()
{
  *this = timestamp(3); // default precision is milliseconds
}

inline timestamp::timestamp(int precision) 
{ 
  getSystemTime(tv_sec, tv_nsec);
  int _precision = precision >= 0 ? precision : 3;  // precision must be positive or zero
  int pw = static_cast<int>( pow(10, _precision) + 0.5 );
  int rnd = _precision <=9 ? 1000000000/pw : 1; 
  tv_nsec = tv_nsec/rnd*rnd;
}

//!< Constructor from numeric values
inline timestamp::timestamp(time_t _tv_sec, long _tv_nsec)
{
  tv_sec = _tv_sec;
  tv_nsec = _tv_nsec;
}

//! Constructor from const char* representation in UTC format
inline timestamp::timestamp(const char* sTime)
{
  *this = fromString(sTime);
}

inline timestamp::timestamp(const std::string& sTime) 
{
  *this = fromString(sTime);
}

inline timestamp::timestamp(const tm& tmTime, int nsec)
{
  tm tmTimeTmp = tmTime;
  tv_sec = mktime(&tmTimeTmp);
  tv_nsec = nsec;
  if(tv_sec<0)
    *this = null();

}

//!< Set non-local time bias and recalculate time value to local
inline timestamp& timestamp::set_bias(const std::string& sBias)
{
  *this += (bias(sBias) - bias()) * 60;
  return *this;
}

inline timestamp::timestamp(int year, int mon, int day, 
    int hour, int min, int sec, 
    int nsec,
    const std::string& sBias
)
{
  tm tmLocal = { sec, min, hour, day, mon, year };
  tv_sec = mktime(&tmLocal);
  tv_nsec = nsec;
  if(tv_sec<0)
    *this = null();
  set_bias(sBias);
}

inline std::string timestamp::timeZone()
{
  return timeZoneString();
}

inline const long timestamp::maxNano()
{
  return 1000000000;
}

inline long timestamp::bias(const std::string& sTimeZone)
{
  using namespace std;
  string timeZone = sTimeZone=="" ? timestamp::timeZone() : sTimeZone;
  assert(timeZone.length()>=6);
  long bias = stoi(string(timeZone.end()-5, timeZone.end()-3))*60 +
              stoi(string(timeZone.end()-2, timeZone.end()));
  bias *= timeZone[timeZone.length()-6]=='-' ? 1 : -1;
  return bias;
}

//!< Generate std::string for the time in UTC format
inline std::string timestamp::toString() const
{
  if(isNull())
    return std::string("");

  tm* tmLocal = localtime( &tv_sec );
  char dateStr[256] = { 0 }; char dateStrN[256] = { 0 };
  strftime(dateStr, sizeof(dateStr),"%Y-%m-%dT%H:%M:%S", tmLocal);
  if(tv_nsec<=0)
    sprintf(dateStrN, "%s%s", dateStr, timeZone().c_str());
  else
  {
    char nsecStr[256] = { 0 };
    sprintf(nsecStr, "%09ld", tv_nsec);
    // erase ending zeros
    char* ps = nsecStr+strlen(nsecStr)-1;
    while(ps>nsecStr && *ps=='0')
      *ps--=0;
    sprintf(dateStrN, "%s.%s%s", dateStr, nsecStr, timeZone().c_str());
  }

  return std::string(dateStrN);
}

//!< Generate std::string for the time in UTC format
inline std::string timestamp::toString(int precision) const
{
  if(isNull())
    return std::string("");

  tm* tmLocal = localtime( &tv_sec );
  char dateStr[256] = { 0 }; char dateStrN[256] = { 0 };
  strftime(dateStr, sizeof(dateStr),"%Y-%m-%dT%H:%M:%S", tmLocal);
  if(tv_nsec<=0 || precision<=0)
    sprintf(dateStrN, "%s%s", dateStr, timeZone().c_str());
  else
  {
    int _precision = precision<=9 ? precision : 9; // maximum precision is nano
    char nsecFormat[256] = { 0 };
    sprintf(nsecFormat, "%%%02dd", _precision);
    char nsecStr[256] = { 0 };
    int pw = static_cast<int>( pow(10, _precision) + 0.5 );
    int rnd = 1000000000/pw;
    sprintf(nsecStr, nsecFormat, tv_nsec/rnd);
    sprintf(dateStrN, "%s.%s%s", dateStr, nsecStr, timeZone().c_str());
  }

  return std::string(dateStrN);
}

//!< Generate timestamp from std::string representation in UTC format
inline timestamp& timestamp::fromString(const std::string& sTime)
{
  using namespace std;

  // Check if date is date correct, like "2017-12-23T12:40:23.263+03:00"
  if(sTime.length()<25)
  {
    *this = null();
    return *this;
  }
  if(stoi(string(sTime.begin(),sTime.begin()+4)) < 1900) // year is less than 1900
  {
    *this = null();
    return *this;
  }

  tm tmLocal = {
    stoi(string(sTime.begin()+17,sTime.begin()+19)),
    stoi(string(sTime.begin()+14,sTime.begin()+16)),
    stoi(string(sTime.begin()+11,sTime.begin()+13)),
    stoi(string(sTime.begin()+ 8,sTime.begin()+10)),
    stoi(string(sTime.begin()+ 5,sTime.begin()+ 7))-1,
    stoi(string(sTime.begin()+ 0,sTime.begin()+ 4))-1900,
    };
  
  tv_sec = mktime(&tmLocal);

  // process milliseconds
  string s_nsec = sTime[19]=='.' ? string(sTime.begin()+20, sTime.end()-6) : "0";
  while(s_nsec.length()<9) 
    s_nsec+='0';
  tv_nsec = stoi(s_nsec);

  // shift timezone to local
  *this += (bias(sTime)-bias())*60;

  return *this;
}

//!< Generate std::string for the time in UTC format
inline timestamp::operator std::string() const
{
  return toString();
}

inline timestamp::operator tm() const
{
  return *localtime(&tv_sec);
}

inline timestamp& timestamp::operator+=(const timestamp& dtime)
{
  // not implemented yet
  return *this;
}

inline timestamp& timestamp::operator-=(const timestamp& dtime)
{
  // not implemented yet
  return *this;
}

//!< Add time delta (in double seconds) to the current time value
inline timestamp& timestamp::operator+=(double dsec)
{
  long nsec = static_cast<long>(dsec);
  tv_sec += nsec;

  // nano seconds arithmetics
  double decsec = dsec - nsec;
  tv_nsec += static_cast<long>(decsec*maxNano());
  if( tv_nsec > maxNano() )
  {
    tv_nsec -= maxNano();
    tv_sec +=1;
  }
  if( tv_nsec < 0 )
  {
    tv_nsec = maxNano() -tv_nsec ;
    tv_sec -= 1;
  }

  return *this;
}

//!< Subtract time delta  (in double seconds) from the current time value
inline timestamp& timestamp::operator-=(double dsec)
{
  *this += -dsec;
  return *this;
}

// Setting values
inline timestamp& timestamp::set_msec(double msec)
{
  tv_sec += int(msec/1000);
  tv_nsec = long(msec*(maxNano()/1000)+0.5) % maxNano();
  return *this;
}

inline timestamp& timestamp::set_usec(double usec)
{
  tv_sec += int(usec/1000000);
  tv_nsec = long(usec*1000+0.5) % maxNano();
  return *this;
}

inline timestamp& timestamp::set_nsec(long nsec)
{
  tv_sec += int(nsec/maxNano());
  tv_nsec = nsec % maxNano();
  return *this;
}

inline int timestamp::year() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_year;
}

inline int timestamp::mon() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_mon;
}

inline int timestamp::day() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_mday;
}

inline int timestamp::hour() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_hour;
}

inline int timestamp::min() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_min;
}

inline int timestamp::sec() const
{
  tm* tmLocal = localtime( &tv_sec );
  return tmLocal->tm_sec;
}

inline double timestamp::msec() const
{
  return double(tv_nsec)/1000000;
}

inline double timestamp::usec() const
{
  return double(tv_nsec)/1000;
}

inline long timestamp::nsec() const
{
  return tv_nsec;
}

//!< return null value 
inline const timestamp& timestamp::null()
{
  static timestamp nullVal(0,0);
  return nullVal;
}

inline bool timestamp::isNegative() const
{
  return tv_sec < 0;
}

inline bool timestamp::isNull() const
{
  return tv_sec==0 && tv_nsec==0;
};

inline timestamp timestamp::diff(const timestamp& start, const timestamp& stop)
{
  timestamp result;
  if ((stop.tv_nsec - start.tv_nsec) < 0) {
      result.tv_sec = stop.tv_sec - start.tv_sec - 1;
      result.tv_nsec = stop.tv_nsec - start.tv_nsec + 1000000000;
  } else {
      result.tv_sec = stop.tv_sec - start.tv_sec;
      result.tv_nsec = stop.tv_nsec - start.tv_nsec;
  }

  return result;
}

inline std::istream& operator>>(std::istream& str, ar::timestamp& ts)
{
  using namespace std;
  string s; 
  str >> s;
  ts = s;
  return str;  
}

inline std::ostream& operator<<(std::ostream& str, const ar::timestamp& ts)
{
  str << std::string(ts);
  return str;  
}

};  // namespace ar

#endif