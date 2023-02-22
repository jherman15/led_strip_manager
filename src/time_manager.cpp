/**
 * @file time_manager.cpp
 * @author Jarosław Herman
 * @version 1.0
 * @date 2023-02-14
 */

//--------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <time_manager.h>

TimeManager::TimeManager() {
    updateTime();
  }

int TimeManager::getHour() {
    return p_tm->tm_hour;
  }

int TimeManager::getMinute() {
    return p_tm->tm_min;
  }

void TimeManager::updateTime() {
    now = time(nullptr);
    p_tm = localtime(&now);
  }