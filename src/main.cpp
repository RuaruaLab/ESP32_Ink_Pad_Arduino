#include <Arduino.h>
#include "PINOUT.h"
#include "EPD_2in13g.h"
#include "imagedata.h"
#include "e_ink.hpp"

INK_Display EInk;

int system_init()
{
  pinMode(LED_PIN, OUTPUT);
  
  return 0;
}


void setup() {
  system_init();
  
  for(int i=0; i<124; i++){
    EInk.img_pixel_set(i, i*2, PIXEL_BLACK);
  }
  EInk.refresh();
}

void loop() {
  // epd.Clear(WHITE);
  // delay(2000);

  // epd.Display(gImage_2in13g);
  // delay(2000);

  // epd.Clear(WHITE);
  // delay(2000);
}