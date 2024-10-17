#include <Arduino.h>
#include "EPD_2in13g.h"

/*像素颜色*/
#define PIXEL_BLACK   0x00
#define PIXEL_WHITE   0X01
#define PIXEL_YELLOW  0x02
#define PIXEL_RED     0x03
/******像素定义*******
* 单个像素：
* 0x00 : 黑
* 0x01 : 白
* 0x02 : 黄
* 0x03 : 红
* 扫描方式：从下往上，从左往右
* 屏幕分辨率：124(rol)*250(col)
*/


class INK_Display
{
private:
    Epd epd;                    //墨水屏操作类
    uint8_t IMG_buffer[7750];   //图像缓冲区

public:
    void refresh();            //将缓冲区中图片刷新到墨水屏
    void img_clear();       //清空缓冲区到全白
    void img_pixel_set(int rol, int col, uint8_t color);  //设定缓冲区像素颜色
    INK_Display();     //初始化epd
    ~INK_Display();
};

