#include <Arduino.h>
#include "PINOUT.h"
#include "EPD_2in13g.h"
#include "display.hpp"


void setup() {
    // pinMode(interruptPin, INPUT_PULLUP);

    /*墨水屏初始化*/
    display.begin();
    display.clearScreen(EPaperDisplay::PIXEL_WHITE);

    for(int i = 0; i < 124; i++) {
        display.setPixel(i, i, EPaperDisplay::PIXEL_RED);
    }
    display.refreshScreen();
}

void loop() {
  display.sleep();
}
