#include <Arduino.h>
#include "PINOUT.h"
#include "EPD_2in13g.h"
#include "imagedata.h"

TaskHandle_t ENS_HandleTask = NULL;
TaskHandle_t LED_HandleTask = NULL;

Epd epd;

int system_init()
{
  pinMode(LED_PIN, OUTPUT);

  epd.Init();
  return 0;
}


void setup() {
  if(system_init() != 0)
  {
    printf("e-Paper init fail!\n");
  }else{
    printf("e-paper init ok\n");
  }
  epd.Display(gImage_2in13g);
}

void loop() {
  // epd.Clear(WHITE);
  // delay(2000);

  // epd.Display(gImage_2in13g);
  // delay(2000);

  // epd.Clear(WHITE);
  // delay(2000);

  epd.Sleep();
}