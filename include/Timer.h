#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include "Button.h"

#define TIMER_RUNNING          0
#define TIMER_STOPPED          1
#define SHOW_TENTHS_OF_SECONDS 0

#define SLEEP_TIMEOUT 10 * 1000 // 31 seconds

class Timer
{
public:
    Timer (Button *button);
    void   update (void);
    String millisToTime (unsigned long milliseconds);
    String getElapsedTime ();
    int    getTimerState ();

private:
    int           mTimerState;
    unsigned long mStartTime;
    Button*       mpButton;
};

#endif /* TIMER_H */