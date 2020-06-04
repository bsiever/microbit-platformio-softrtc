#include "softrtc.h"

#include "Microbit.h"
#include "MicroBitSystemTimer.h"


// Static variable(s) for "zero time"
static time_t timeAtSetpoint = 0;
static uint64_t setpoint = 0; 
// The last read time
static bool running = false; 
static struct tm currentTime;

static void updateCurrentTimeUsing(time_t aTime) {

    time_t delta_s = aTime - setpoint;
    
    // time_t driftCompensation = (time_t)((delta_s/(24*60*60.0))*driftPerDay);  // Num of days * comp/day
    
    // + driftCompensation;    
    
    time_t now = timeAtSetpoint + delta_s; 
    gmtime_r(&now, &currentTime);
}


// Must read every hourish to track rollover (Task???) (<71 minutes)
static time_t systemTime() {
    static uint32_t upperTimeUS = 0;
    
    static uint32_t lastTimeUS = 0;
    uint32_t thisTimeUS = us_ticker_read();
    
    // Check for rollover
    if(thisTimeUS<lastTimeUS) {
        upperTimeUS++;
    }
    lastTimeUS = thisTimeUS;

    uint64_t timeUS = ((uint64_t)upperTimeUS)<<32 | ((uint64_t)thisTimeUS);
    return timeUS/1000000;
//    return (time_t)(NRF_RTC1->COUNTER/32768.0);
}


// Create a readCurrentTimeTask that runs every 20 minutes or so. 
void periodicUpdate() {
    while(true) {
        systemTime();
//        fiber_sleep(250);  // Sleep 0.25s seconds
        fiber_sleep(5*60*1000);  // Sleep 5 minutes seconds
    }
}


void ensureUpdates() {
    if(running == false) {
        (void)invoke(periodicUpdate); // create fiber and schedule it.
        running = true;
    }
}

void updateCurrentTime() {
    ensureUpdates();
    updateCurrentTimeUsing(systemTime());
}

void set_date_to(int year, int month, int day) {
    ensureUpdates();
    time_t now = systemTime();
    updateCurrentTimeUsing(now);
    currentTime.tm_year = year-1900;
    currentTime.tm_mon = month-1;
    currentTime.tm_mday = day;
    setpoint = now;
    timeAtSetpoint = mktime(&currentTime);
}

void set_time_to_24hourFormat(int hour, int minute, int sec) {
    ensureUpdates();

    hour = hour%24;
    minute = minute%60;
    sec = sec%60;

    time_t now = systemTime();
    updateCurrentTimeUsing(now);
    currentTime.tm_hour = hour;
    currentTime.tm_min = minute;
    currentTime.tm_sec = sec;
    setpoint = now;
    timeAtSetpoint = mktime(&currentTime);
}

void set_time_to_amPm(int hour, int minute, int sec, bool isAm) {
    ensureUpdates();

    if(isAm && hour==12)  // 12am is 0 hours
        hour = 0;
    else if(!isAm && hour==12)        // 12pm and later -> 13..23
        hour = 12;
    else if(!isAm && hour>12)
        hour = 12 + hour%12;

    set_time_to_24hourFormat(hour+(isAm==false?12:0), minute, sec);
}


bool isAm() {
    return currentTime.tm_hour<12;
}

bool isPm() {
    return !isAm();
}

int getYear() {
    return currentTime.tm_year+1900;
}
int getMonth() {
    return currentTime.tm_mon+1;
}
int getDay() {
    return currentTime.tm_mday;
}

int getWeekday() {
    return currentTime.tm_wday;
}

int getHour24hourFormat() {
    return currentTime.tm_hour;
}

int getDayOfYear() {
    return currentTime.tm_yday;
}

int getHour() {
    int hour = currentTime.tm_hour;
    if(hour == 0)  // 12 am
        return 12;
    if(hour > 12)  
        return hour-12;
    return hour;
}


int getMinute() {
    return currentTime.tm_min;
}

int getSecond() {
    return currentTime.tm_sec;
}

// Adding minutes is moving the setpoint into the past
void advanceMinutes(int m) {
    timeAtSetpoint += m*60;
}

// Adding seconds is moving the setpoint into the past
void advanceSeconds(int s) {
    timeAtSetpoint += s;
}

// Adding hours is moving the setpoint into the past
void advanceHours(int h) {
    timeAtSetpoint += h*60*60;
}

int getRTCSRC() {
    return NRF_CLOCK->CTIV;
//    return NRF_CLOCK->LFCLKSRC;
}


uint32_t getTicker() {
    return us_ticker_read();
}