//!
//!@file    xhelpers/sx_timer.h
//!@author  Sholomov Dmitry
//!@author  Postnikov Vassili
//!@brief   Helper class for timer operations. Analog for XTimer-class in CForms project
//!@note    Implementation is partly based on arcore library, so arcore.lib linking is required
//!

#ifndef SX_TIMER_H
#define SX_TIMER_H

#include <ar10/articker.h>
#include <ar10/sx_string.h>

namespace sx {

//! @class std::xtimer xhelpers/sx_timer.h
//! @brief Класс-хэлпер для замеров времени
class xtimer : public sx::xstring 
{
public:

  xtimer(const char* procId = 0);                 //!< Constructors and destructors
  virtual ~xtimer(void);

  double start(const char* procId = 0);           //!< Начать отсчет
  double stop();                                  //!< Остановить отсчет
  double update();                                //!< Обновить замер времени
  
  double getDuration(bool bUpdate = true);        //!< Время между замерами в msec
  
  const char* getDurationText(                    //!< Текстовое представление времени между замерами
    const char* format = 0,                         // [in] формат текста
    bool bUpdate = true                             // [in] обновлять ли замер или выводить время предыдущего
    );
 
protected:

  double computeDuration();                       //!< Вычисление разницы между замерами

  long long tStart, tFinish;
  double  duration;
  bool    stopped;
  sx::xstring proc;
  sx::xstring message;
};

inline xtimer::xtimer(const char* procId) :
  tStart(0), tFinish(0),
  duration(.0),
  stopped(false),
  proc(""),
  message("")
{
  start(procId);
}

inline xtimer::~xtimer()
{
  stop();
}

//!< Начать отсчет
inline double xtimer::start(const char* procId)
{
  tStart=0; tFinish=0;
  if (procId)
    proc = procId;
  else
    proc = "Еlapsed time";

  tStart = static_cast<long long>(ar::Clock::getTickCount());
  stopped = false;
  return static_cast<double>(tStart);
};

//!< Остановить отсчет
inline double xtimer::stop()
{ 
  if (!stopped)
    update();
  stopped = true;
  return duration;
}

//!< Обновить замер времени
inline double xtimer::update() 
{  
  tFinish = static_cast<long long>(ar::Clock::getTickCount());
  duration = computeDuration();
  std::string& strRef = *this;
  strRef = getDurationText(0, false);
  tStart = static_cast<long long>(ar::Clock::getTickCount());
  return duration; 
};

//!< Время между замерами в msec
inline double xtimer::getDuration(bool bUpdate)
{
  if(!stopped && bUpdate)
      update();

  return duration;
}

//!< Текстовое представление времени между замерами
inline const char* xtimer::getDurationText(const char* format, bool bUpdate)
{
  message = sx::xstring().format(format ? format : "%2.2fms", getDuration(bUpdate));
  return message.c_str();
}

inline double xtimer::computeDuration() 
{
  return (tFinish-tStart) / static_cast<double>(ar::Clock::getTickFrequency()) * 1000.;
}

}; // namespace sx

#endif // SX_TIMER_H
