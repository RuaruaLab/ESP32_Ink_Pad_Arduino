#include "EPD_2in13g.h"
#include "imagedata.h"

#ifndef EPAPER_DISPLAY_HPP
#define EPAPER_DISPLAY_HPP

/*显示缓冲区*/
uint8_t IMG_buffer[7750];            //图片缓冲区

class EPaperDisplay {
private:
    Epd epd;
    uint8_t *img_buffer;

    static const int ROWS = 124;
    static const int COLS = 250;

public:
    enum PixelColor {
        PIXEL_BLACK = 0x00,
        PIXEL_WHITE = 0x01,
        PIXEL_YELLOW = 0x02,
        PIXEL_RED = 0x03
    };

    // 构造函数，不再执行初始化操作
    EPaperDisplay(uint8_t *buffer) : img_buffer(buffer) {}

    // 显式初始化函数
    void begin() {
        memset(img_buffer, 0xFF, sizeof(IMG_buffer)); // 初始化为全白
        if (epd.Init() != 0) {
            Serial.begin(9600);
            Serial.println("e-Paper init failed!");
        } else {
            Serial.begin(9600);
            Serial.println("e-Paper init ok");
        }
    }

    //设置像素
    void setPixel(int row, int col, PixelColor color) {
        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            return; // 越界检查
        }
        int index = row + col * ROWS;
        int byte_index = index / 4;
        int bit_offset = 6 - (index % 4) * 2;

        img_buffer[byte_index] &= ~(0x03 << bit_offset);
        img_buffer[byte_index] |= (color << bit_offset);
    }

    void clearScreen(PixelColor color = PIXEL_WHITE) {
        uint8_t packedColor = (color << 6) | (color << 4) | (color << 2) | color;
        for (int i = 0; i < sizeof(IMG_buffer); i++) {
            img_buffer[i] = packedColor;
        }
    }

    //刷新成缓冲区内的图像
    void refreshScreen() {
        epd.Display(img_buffer);
    }

    //直接通过图像刷新屏幕
    void set(const UBYTE *Image){
        epd.Display(Image);
    }

    void sleep() {
        epd.Sleep();
    }
};

/*墨水屏接口*/
EPaperDisplay display(IMG_buffer);   //显示控制类

#endif