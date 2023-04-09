#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#include "Display.h"
#include "Timer.h"
#include "Battery.h"
#include "Button.h"

#define MAX_FONTS 4
GFXfont fonts [MAX_FONTS] = {FreeSans9pt7b, FreeSans12pt7b, FreeSans18pt7b, FreeSans24pt7b};

Display::Display (void)
{
    mpScreen = new Adafruit_SSD1306 (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    if (!mpScreen->begin (SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println (F ("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // mpScreen->setFont (&FreeSans24pt7b);
    mpScreen->clearDisplay ();
    mpScreen->ssd1306_command(SSD1306_DISPLAYON);
    mpScreen->setTextWrap(false);

    mpBattery = new Battery (3200, 4200, BATTERY_VOLTAGE_PIN);
    mpBattery->begin (3300, 1.47, &sigmoidal);

    mpButton = new Button ();
    mpTimer = new Timer (mpButton);

    mpBatteryPercentageText = new TextDisplay (BATTERY_PERCENTAGE_X, BATTERY_PERCENTAGE_Y, BATTERY_PERCENTAGE_WIDTH, BATTERY_PERCENTAGE_HEIGHT);
    mpTimeText = new TextDisplay (TIME_X, TIME_Y, TIME_WIDTH, TIME_HEIGHT);
}

void Display::update (void)
{
    unsigned long now = millis ();
    unsigned long last = mpButton->getLastButtonPressTime ();
    if (last + SLEEP_TIMEOUT < now && mpTimer->getTimerState () == TIMER_STOPPED)
    {
        mpScreen->ssd1306_command(SSD1306_DISPLAYOFF);
        esp_deep_sleep_start();
    }

    mpTimer->update ();
    displayBatteryPercentage ();
    displayTime ();
}

void Display::displayTime (void)
{
    String timeText = mpTimer->getElapsedTime ();
    if (timeText.length () == 0)
    {
        timeText = "-:--";
        mpTimeText->reset ();
        mpScreen->dim (true);
    }
    else
    {
        mpScreen->dim (false);
    }

    mpTimeText->update (timeText);
}

void Display::displayBatteryPercentage ()
{
    char percentageText[12];
    int batteryPercentage = mpBattery->level ();
    drawBattery (0, 19, 40, 12, batteryPercentage);

    sprintf (percentageText, "%d%%", batteryPercentage);
    mpBatteryPercentageText->update (String (percentageText));
}

void Display::drawBattery (int x, int y, int batteryWidth, int batteryHeight, int batteryPercentage)
{
    int i;
    
    mpScreen->fillRect (x, y, batteryWidth, batteryHeight, SSD1306_BLACK);
    mpScreen->drawRect (x, y, batteryWidth - BATTERY_TIP_WIDTH, batteryHeight, SSD1306_WHITE);
    mpScreen->drawRect (x + 1, y + 1, batteryWidth - BATTERY_TIP_WIDTH - 2, batteryHeight - 2, SSD1306_WHITE);
    int tipGap = batteryHeight / 4;
    for (int i = 0; i < BATTERY_TIP_WIDTH; i++)
    {
        mpScreen->drawFastVLine (x + i + batteryWidth - BATTERY_TIP_WIDTH, y + tipGap, batteryHeight - (tipGap * 2), SSD1306_WHITE);
    }

    int total_bars = batteryWidth - 8;
    int drawn_bars = total_bars * batteryPercentage / 100;
    for (int i = 0; i < drawn_bars; i++)
    {
        mpScreen->drawFastVLine (x + i + 5, y + 4, y + batteryHeight - 8, SSD1306_WHITE);
    }
}

void Display::errorMessage (String message)
{
    mpScreen->clearDisplay ();
    mpScreen->setFont (&FreeSans9pt7b);
    mpScreen->setTextSize (1);
    mpScreen->setTextColor (WHITE);
    mpScreen->setCursor (0, SCREEN_HEIGHT - 1);
    mpScreen->println (message);
    mpScreen->display ();

    delay (10000);
    mpScreen->clearDisplay ();
}

float Display::readBatteryVoltage (void)
{
#define LAST_READS_COUNT           50
#define BATTERY_VOLTAGE_MULTIPLIER 0.001201729

    static int lastReads[LAST_READS_COUNT] = { 0 };
    static int counter                      = 0;
    int        i;
    float      voltage;

    int number            = analogRead (BATTERY_VOLTAGE_PIN);
    lastReads[counter++] = number;
    if (counter == LAST_READS_COUNT)
    {
        counter = 0;
    }

    number = 0;
    for (i = 0; i < LAST_READS_COUNT; i++)
    {   
        number += lastReads[i];
    }
    number /= LAST_READS_COUNT;
    voltage = (float)number * BATTERY_VOLTAGE_MULTIPLIER;
    return (voltage);
}

TextDisplay::TextDisplay (int x, int y, int width, int height)
{
    setCoordinates (x, y, width, height);
    reset ();
}

void TextDisplay::setCoordinates (int x, int y, int width, int height)
{
    mTextX = x;
    mTextY = y;
    mTextWidth = width;
    mTextHeight = height;
}

void TextDisplay::update (String text)
{
    int fontIndex = mMaxFontIndex;
    int16_t  actualTextX, actualTextY;
    uint16_t actualTextWidth, actualTextHeight;
    extern Display* pDisplay;

    pDisplay->mpScreen->fillRect (mTextX, mTextY - mTextHeight, mTextWidth, mTextHeight, SSD1306_BLACK);
    pDisplay->mpScreen->setTextSize (1);
    pDisplay->mpScreen->setTextColor (SSD1306_WHITE, SSD1306_BLACK);
    do
    {
        pDisplay->mpScreen->setFont (&(fonts [fontIndex]));
        pDisplay->mpScreen->getTextBounds(text, mTextX, mTextY, &actualTextX, &actualTextY, &actualTextWidth, &actualTextHeight);
        // Serial.println (String (fontIndex) + ": " + String (timeWidth));
        if (fontIndex < mMaxFontIndex)
        {
            mMaxFontIndex = fontIndex;
        }
    } while (fontIndex-- > 0 && (actualTextWidth > mTextWidth|| actualTextHeight > mTextHeight));
    
    pDisplay->mpScreen->setCursor (mTextX + ((mTextWidth - actualTextWidth)) / 2, mTextY + 1 - ((mTextHeight - actualTextHeight)) / 2);
    pDisplay->mpScreen->println (text);
    pDisplay->mpScreen->display ();
}

void TextDisplay::reset (void)
{
    mMaxFontIndex = MAX_FONTS - 1;
}
