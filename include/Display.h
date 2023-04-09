#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Battery.h"
#include "Timer.h"

#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define BATTERY_VOLTAGE_PIN    33
#define BATTERY_CHECK_INTERVAL 30000

#if SCREEN_HEIGHT == 32
#define TIME_X      45
#define TIME_Y      SCREEN_HEIGHT - 1
#define TIME_WIDTH  SCREEN_WIDTH - TIME_X - 1
#define TIME_HEIGHT SCREEN_HEIGHT - 1

#define BATTERY_PERCENTAGE_X      0
#define BATTERY_PERCENTAGE_Y      14
#define BATTERY_PERCENTAGE_WIDTH  35
#define BATTERY_PERCENTAGE_HEIGHT 16

#define BATTERY_X         0
#define BATTERY_Y         20
#define BATTERY_WIDTH     35
#define BATTERY_HEIGHT    12
#define BATTERY_TIP_WIDTH 4
#elif SCREEN_HEIGHT == 64

#endif

class TextDisplay
{
public:
    TextDisplay (int x, int y, int width, int height);
    void update (String text);
    void reset (void);
    void setCoordinates (int x, int y, int width, int height);

private:
    int mMaxFontIndex;
    int mTextX, mTextY;
    int mTextWidth, mTextHeight;
};

class Display
{
public:
    Display (void);
    void  update (void);
    void  displayTime (void);
    void  displayBatteryPercentage (void);
    void  drawBattery (int x, int y, int batteryWidth, int batteryHeight, int batteryPercentage);
    void  errorMessage (String message);
    float readBatteryVoltage (void);

    Adafruit_SSD1306* mpScreen;

protected:

private:
    Battery*          mpBattery;
    Timer*            mpTimer;
    Button*           mpButton;
    TextDisplay*      mpBatteryPercentageText;
    TextDisplay*      mpTimeText;
};



#endif /* DISPLAY_H */