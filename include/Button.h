#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_GPIO_PIN      25
#define BUTTON_DEBOUNCE_TIME 250

class Button
{
public:
    Button (void);
    bool hasButtonBeenPressed (void);
    unsigned long getLastButtonPressTime (void);

private:
    unsigned long mLastButtonPressTime;
};

#endif /* BUTTON_H */