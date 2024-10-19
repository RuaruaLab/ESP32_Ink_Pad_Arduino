#include <Arduino.h>
#include "PINOUT.h"
#include "display.hpp"

/*系统状态*/
enum SystemStatus {
    NORMAL = 0x00,
    USB_EXTRACT = 0x01
};
uint8_t sys_status = NORMAL;

void usb_extract_handle()
{
  if(digitalRead(V_3125_PIN) == LOW) sys_status = USB_EXTRACT;
  else if(digitalRead(V_3125_PIN) == HIGH) sys_status = NORMAL;
}

void setup() {
    /*GPIO初始化*/
    //LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    //USB拔出检测
    pinMode(V_3125_PIN, INPUT);
    attachInterrupt(V_3125_PIN, usb_extract_handle, CHANGE);
    /*墨水屏初始化*/
    display.begin();
    display.clearScreen(EPaperDisplay::PIXEL_WHITE);

    for(int i = 0; i < 124; i++) {
        display.setPixel(i, i, EPaperDisplay::PIXEL_RED);

    }
    // display.refreshScreen();
    // display.set(gImage_2in13g);
}

void loop() {
  /*USB拔出处理*/
  if(sys_status == USB_EXTRACT)
  {
    display.set(gImage_2in13g);
    while(1)
    {
      delay(100);
      if(sys_status == NORMAL) break;
    }
  }
  
}
