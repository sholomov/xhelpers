//!
//!@file    xhelpers/sx_cast.h
//!@author  Sholomov Dmitry
//!@brief   Extra types casting functions
//!

#ifndef SX_CAST_H
#define SX_CAST_H

#include <string>
#include <cstring>

namespace sx {

inline int int_cast(const std::string& str)
{
    if(str.length()>1 && str[1]=='x') //hex
    {
        int num=0;
        sscanf(str.c_str(),"0x%x",&num);
        return num;
    }
    return atoi(str.c_str());
}

inline unsigned int uint_cast(const std::string& str)
{
    return static_cast<unsigned int>(atoi(str.c_str()));
}

inline double double_cast(const std::string& str)
{
    return static_cast<double>(atof(str.c_str()));
}

inline std::string _string_cast(long num,int radix=10)
{
  char szStr[16] = {0};
  switch (radix)
    {
    case 8: sprintf(szStr, "%o", static_cast<unsigned int> (num)); break;
    case 10: sprintf(szStr, "%d", static_cast<int> (num)); break;
    case 16: sprintf(szStr, "%x", static_cast<unsigned int> (num)); break;
    default:
      {
        std::string str;
        long n = abs(num);
        while (n)
          {
            long rem = n % radix;
            n /= radix;
            if (rem < 10) str = char(rem - '0') + str;
            else str = char(rem - 'a') + str;
          }
        return str;
      }
    }
    return szStr;
}

inline std::string string_cast(int num,int radix=10)
{
  return _string_cast(num, radix);
}

inline std::string string_cast(long num,int radix=10)
{
  return _string_cast(num, radix);
}

inline std::string string_cast(double num)
{
  #ifndef _CVTBUFSIZE
  #define _CVTBUFSIZE (309+40) /* # of digits in max. dp value + slop */
  #endif
  char szStr[_CVTBUFSIZE] = {0};
#ifdef WIN32
  _gcvt(num,24,szStr);
#else
  gcvt(num,24,szStr);
#endif
  if(szStr[0]&& szStr[strlen(szStr)-1]=='.')
    szStr[strlen(szStr)-1]=0;
  std::string str(szStr);
  return str;
}

inline std::string string_cast(double num, const char* format)
{
  char szStr[24] = {0};
  sprintf(szStr, format, num);
  return szStr;
}

inline std::string string_cast(unsigned int num)
{
    return string_cast((int)num);
}

inline std::string string_cast(unsigned long num)
{
    return string_cast((long)num);
}

}; // namespace sx;


#endif // SX_CAST_H
