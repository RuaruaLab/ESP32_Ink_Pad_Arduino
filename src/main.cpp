#include <Arduino.h>
#include "PINOUT.h"
#include "display.hpp"

void usb_extract_handle()
{
  display.refreshScreen();
  while (1){}
}

void setup() {
    /*GPIO初始化*/
    //LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    //USB拔出检测
    pinMode(V_3125_PIN, INPUT);
    attachInterrupt(V_3125_PIN, usb_extract_handle, FALLING);
    /*墨水屏初始化*/
    display.begin();
    display.clearScreen(EPaperDisplay::PIXEL_WHITE);

    for(int i = 0; i < 124; i++) {
        display.setPixel(i, i, EPaperDisplay::PIXEL_RED);

    }
}

void loop() {
}
