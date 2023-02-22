/**
 * @file time_manager.h
 * @author Jarosław Herman
 * @version 1.0
 * @date 2023-02-14
 */

//--------------------------------------------------------------------------------

#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <ESP8266WiFi.h>
#include <time.h>

class TimeManager {
public:
  time_t now;
  struct tm* p_tm;
  const int timezone = 1 * 3600;
  const int dst = 0;
  int hour_flag{};
  
  TimeManager();

  int getHour();

  int getMinute();

  void updateTime();

};

#endif