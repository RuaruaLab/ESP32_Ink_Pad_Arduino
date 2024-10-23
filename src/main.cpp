#include <Arduino.h>
#include "PINOUT.h"
#include "display.hpp"

uint8_t recive_buff[256] = {0};

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

void serial_ubpack(uint8_t *pack);

void setup() {
    /*GPIO初始化*/
    //LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    //USB拔出检测
    pinMode(V_3125_PIN, INPUT);
    // attachInterrupt(V_3125_PIN, usb_extract_handle, CHANGE);
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
  // if(sys_status == USB_EXTRACT)
  // {
  //   display.set(gImage_2in13g);
  //   while(1)
  //   {
  //     delay(100);
  //     if(sys_status == NORMAL) break;
  //   }
  // }

  /*
  通讯协议：
  第一位  帧头 0x5C
  第二位  数据长度，不含帧头(256以内)
  第三位  CMD_ID
  <数据段>
  末位    帧尾 0x7A
  */
  /*接收数据包*/
  if(Serial.available() > 0){
    /*识别帧头*/
    uint8_t read_data;
    read_data = Serial.read();
    if(read_data == 0x5C)
    {
      memset(recive_buff, 0, sizeof(recive_buff));  //清零缓冲区

      uint8_t data_length = 0;
      data_length = Serial.read();
      Serial.readBytes(recive_buff, data_length);
      if(recive_buff[data_length-1] == 0x7A)
      {
        serial_ubpack(recive_buff);
      }
    }
  }

  // printf("main task ok\n");
  delay(1);
  
}

void serial_ubpack(uint8_t *pack)
{
  printf("%s\n", pack);
  return ;
}