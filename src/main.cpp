// Docket Timer program V0.3
// Written by John Robinson for Didier Richard.
// 6th November 2022
#include <Arduino.h>
// #include "WiFi.h"
#include "Display.h"
#include "Button.h"
// #include <esp_wifi.h>

Display* pDisplay;

// All Arduino programs must have two functions present. The first is called "setup", and it is called just
// once at the start of the program. The other is called "loop" (see below) which just loops over and over
// doing whatever it is meant to do

void setup ()
{
    Serial.begin (9600);

    pDisplay = new Display ();

    // We need to throttle the CPU frequency just to save battery life
    setCpuFrequencyMhz (20);
    Serial.print ("CPU Freq: ");
    Serial.println (getCpuFrequencyMhz ());

    // WiFi.setSleep(true);
    //         // adc_power_off();
    // WiFi.disconnect(true);  // Disconnect from the network
    // WiFi.mode(WIFI_OFF);
    // btStop();
    // TODO: need to look at turning off Wifi and Bluetooth

    esp_sleep_enable_ext0_wakeup ((gpio_num_t) BUTTON_GPIO_PIN, HIGH);

    Serial.println ("Setup Finished");
}

void loop ()
{
    pDisplay->update ();
}

// TODO: Need to look at putting ESP32 to sleep, and turning off display
