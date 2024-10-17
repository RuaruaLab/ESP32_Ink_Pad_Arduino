#include <Arduino.h>
#include "PINOUT.h"
#include "EPD_2in13g.h"
#include "imagedata.h"

TaskHandle_t ENS_HandleTask = NULL;
TaskHandle_t LED_HandleTask = NULL;

Epd epd;

/*图片缓冲区*/
/******像素定义*******
* 单个像素：
* 0x00 : 黑
* 0x01 : 白
* 0x02 : 黄
* 0x03 : 红
* 扫描方式：从下往上，从左往右
* 屏幕分辨率：124(rol)*250(col)
*/
uint8_t IMG_buffer[7750];
#define PIXEL_BLACK   0x00
#define PIXEL_WHITE   0X01
#define PIXEL_YELLOW  0x02
#define PIXEL_RED     0x03

//设定任意个像素的颜色
// 设置指定位置像素的颜色
void img_pixel_set(uint8_t *img, int rol, int col, uint8_t color)
{
    //越界检查
    if(rol < 0 || rol >= 124 || col < 0 || col >= 250) {
        return;
    }
    //计算索引
    int index = rol + col * 124;
    int byte_index = index / 4;
    int bit_offset = 6 - (index % 4) * 2;
    // int bit_offset = 0;
    
    //设定颜色
    img[byte_index] &= ~(0x03 << bit_offset);
    img[byte_index] |= (color << bit_offset);
}

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

  /*设置为全白*/
  int pixel_vel;
  for(int i=0; i<sizeof(IMG_buffer); i++){
    pixel_vel = PIXEL_WHITE;
    IMG_buffer[i] = pixel_vel << 6 | pixel_vel << 4 | pixel_vel << 2 | pixel_vel;
  }

  /*画一个y=x的线*/
  for(int i=0; i<124; i++){
    img_pixel_set(IMG_buffer, i, i, PIXEL_RED);
  }

  // IMG_buffer[50] = pixel_vel << 6 | pixel_vel << 4 | pixel_vel << 2 | pixel_vel;

  // img_pixel_set(IMG_buffer, 50, 50, PIXEL_RED);

  epd.Display(IMG_buffer);
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