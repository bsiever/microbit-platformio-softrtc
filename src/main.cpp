#include <mbed.h>

#include "MicroBit.h"

#include "softrtc.h"
MicroBit uBit;

void onConnected(MicroBitEvent) {
    uBit.display.printChar('O');
    uBit.serial.printf("Connected\n");
}

void onDisconnected(MicroBitEvent) {
    uBit.display.printChar('X');
    uBit.serial.printf("Disconnected\n");
}

void showTime() {
    int lastUpdate = 0;
    while(true) {
        fiber_sleep(200);
        updateCurrentTime();
        int currentSeconds = getSecond();
        if(currentSeconds!=lastUpdate) {
            uBit.serial.printf("Date: %04d-%02d-%02d\n", getYear(), getMonth(), getDay());
            uBit.serial.printf("Time: %d:%02d:%02d %sm\n", getHour(), getMinute(), currentSeconds, isAm()?"a":"p");
            lastUpdate = currentSeconds;
        }
    }
}


void displayTimeFor(int seconds) {
    int lastUpdate = 0;
    while(seconds>0) {
        fiber_sleep(100);
        updateCurrentTime();
        int currentSeconds = getSecond();
        if(currentSeconds!=lastUpdate) {
            uBit.serial.printf("Date: %04d-%02d-%02d\n", getYear(), getMonth(), getDay());
            uBit.serial.printf("Time: %d:%02d:%02d\n", getHour24hourFormat(), getMinute(), currentSeconds);
//            uBit.serial.printf("Time: %d:%02d:%02d %sm\n", getHour(), getMinute(), currentSeconds, isAm()?"a":"p");
            lastUpdate = currentSeconds;
            seconds--;
        }
    }
}


void onButton(MicroBitEvent e) {
    displayTimeFor(10);
}

int main() {
    uBit.init();

//    uBit.display.printChar('-');
    uBit.serial.printf("Start......\n");
    set_date_to(2020, 6, 4);
    set_time_to_24hourFormat(23, 59, 50);  
    displayTimeFor(30);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButton);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}










/**
 * 

//set_time_to(0,0,0,0,0,0);

//876.5217

    //int i=0;
//     int lastUpdate = 0;
//     while(true) {
//         fiber_sleep(200);

// // uint32_t ticks = 0;
// // app_timer_cnt_get(&ticks);
// // uBit.serial.printf("App Timer: %d\n", ticks);
// // uint32_t counter = NRF_RTC1->COUNTER;
// // uBit.serial.printf("RTC Counter1: %d\n",counter);
// // float time = counter / 32768.0 * 1000.0;
// // uint32_t timeMs = (uint32_t)time;
// // uBit.serial.printf("RTC Time1: %d\n",timeMs);

// // uint32_t counter0 = NRF_RTC0->COUNTER;
// // uBit.serial.printf("RTC Counter0: %d\n",counter0);
// // float time1 = counter0 / 32768.0 * 1000.0;
// // uint32_t timeMs1 = (uint32_t)time1;
// // uBit.serial.printf("RTC Time0: %d\n",timeMs1);


// // uBit.serial.printf("System: %d\n",system_timer_current_time());
// //uBit.serial.printf("Ticker: %d ms\n",us_ticker_read()/1000000);



// //        uBit.display.scroll(i);  // Scrolling takes time...
//         // uBit.serial.printf("Long Bytes : %d\n",sizeof(unsigned long));
//         // uBit.serial.printf("uint64 Bytes : %d\n",sizeof(uint64_t));
//         // uBit.serial.printf("Time_t Bytes : %d\n",sizeof(time_t));
//         // uBit.serial.printf("Count : %d\n",uBit.systemTime());
//         // uBit.serial.printf("RTCSRC: %d\n", );
//         // uBit.serial.printf("Interval: %d\n", getRTCSRC());
//         updateCurrentTime();
//     int currentSeconds = getSecond();
//     if(currentSeconds!=lastUpdate) {
//         uBit.serial.printf("Date: %04d-%02d-%02d\n", getYear(), getMonth(), getDay());
//         uBit.serial.printf("Time: %d:%02d:%02d %sm\n", getHour(), getMinute(), currentSeconds, isAm()?"a":"p");
//         lastUpdate = currentSeconds;
//     }

// //rtc1_getCounter64()
// //RTC_UNITS_TO_MICROSECONDS

//     }


//    invoke(showTime);



 * 
 * */