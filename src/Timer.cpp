
#include "Timer.h"
#include "Button.h"

Timer::Timer (Button *button)
{
    mTimerState = TIMER_STOPPED;
    mpButton = button;
}

void Timer::update (void)
{
    if (mpButton->hasButtonBeenPressed ())
    {
        if (mTimerState == TIMER_RUNNING)
        {
            mTimerState = TIMER_STOPPED;
        }
        else
        {
            mTimerState = TIMER_RUNNING;
            mStartTime  = millis ();
        }
    }
}

String Timer::getElapsedTime ()
{
    if (mTimerState == TIMER_RUNNING)
    {
        return (millisToTime (millis () - mStartTime));
    }
    else
    {
        return "";
    }
}

int Timer::getTimerState ()
{
    if (mTimerState == TIMER_RUNNING)
    {
        return (TIMER_RUNNING);
    }

    return TIMER_STOPPED;
}

String Timer::millisToTime (unsigned long milliseconds)
{
    int  hours    = 0;
    int  minutes  = 0;
    int  seconds  = 0;
    int  decimals = 0;
    char timeString[20];

    // The current time will be a count of milliseconds since we started, so for example, if
    // might be "7654321", but that's not very useful, so we need to convert it to hours, minutes
    // seconds and tenths of seconds
    // First we know that the last three digits are the thousandths of a second, in this exacmple
    // "321". so we will grab those three digits, convert it to tenths of a second eg "3", and then
    // remove those last three digits from the current time. This means the current time is now the
    // number of seconds (no longer milliseconds), in this example "7654" (and we have 3 tenths
    // of a second saved).
    decimals = milliseconds % 1000;
    decimals /= 100;
    milliseconds /= 1000;

    // so now we have a current_time of 7654 seconds, which we need to convert to hours, minutes
    // and seconds. If we divide the current time by 60 (since there are 60 seconds in a minute),
    // the result will be the number of minutes, and the remainder will be the number of seconds
    // So, 7654 / 60 gives us 127 minutes, and 34 seconds, so we'll store the 34 seconds.
    seconds = milliseconds % 60;
    milliseconds /= 60;

    // Now the current time is the number of minutes sonce we started, in this case 127. If we
    // divide that by 60 (since there are 60 minutes in an hour) the result will be the number
    // of hours and the remainder will be the number of minutes
    // So, 127 minutes divided by 60 leaves 2 hours, and the remainder is 7 minutes
    minutes = milliseconds % 60;
    milliseconds /= 60;

    hours = milliseconds;
    // So now we have converted the number of milliseconds (7654321) into a more readable 2 hours,
    // 7 minutes 34 seconds and 3 tenths, and so thats how we will display it

    // Now we just need to convert those numbers into a readable string in the format "hh:mm:ss.t",
    // In this case, that is "2:07:34.3"
    if (hours)
    {
        sprintf (timeString, "%d:%02d:%02d", hours, minutes, seconds);
    }
    else
    {
        if (minutes < 10 && SHOW_TENTHS_OF_SECONDS)
        {
            sprintf (timeString, "%d:%02d.%1d", minutes, seconds, decimals);
        }
        else
        {
            sprintf (timeString, "%d:%02d", minutes, seconds);
        }
    }
    return String (timeString);
}
