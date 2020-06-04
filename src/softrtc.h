#ifndef SOFTRTC_H
#define SOFTRTC_H

#include<time.h>
void updateCurrentTime();
void set_date_to(int year, int month, int day);
void set_time_to_24hourFormat(int hour, int minute, int sec);
void set_time_to_amPm(int hour, int minute, int sec, bool isAm);
bool isAm();
bool isPm();
int getYear();
int getMonth();
int getDay();
int getWeekday();
int getDayOfYear();
int getHourMilitary();
int getHour24hourFormat();
int getHour();
int getMinute();
int getSecond();
void advanceMinutes(int m);
void advanceSeconds(int s);
void advanceHours(int h);
int getRTCSRC();
uint32_t getTicker();
#endif 