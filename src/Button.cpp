#include <Arduino.h>
#include "Button.h"

void buttonPress (void);
bool buttonPressedFlag;

Button::Button ()
{
    buttonPressedFlag = false;

        // The button "Interupt Driven", we can configure the processor so that when the
    // Button is pressed, it will interrupt the processor from what it is doing, and make it take action for the
    // button press. We need to set up an Interrupt here to call the function "button_press()" (see below)
    // whenever the button is pressed.
    pinMode (BUTTON_GPIO_PIN, INPUT_PULLUP);
    attachInterrupt (digitalPinToInterrupt (BUTTON_GPIO_PIN), buttonPress, HIGH);

    delay (BUTTON_DEBOUNCE_TIME);
}

bool Button::hasButtonBeenPressed ()
{
    if (buttonPressedFlag)
    {
        buttonPressedFlag = false;
        mLastButtonPressTime = millis ();
        delay (BUTTON_DEBOUNCE_TIME);
        return true;
    }
    return false;
}

unsigned long Button::getLastButtonPressTime (void)
{
    return mLastButtonPressTime;
}

// This is the routine that will be called by the interupt we set up when the button is pressed.
// We always want to keep interupt routines as short as possible, so all we do is update the state
// of the state machine, and set the flag "button_pressed" to true, so that the main loop
// can deal with it next time around
void buttonPress (void)
{
    buttonPressedFlag = true;
}