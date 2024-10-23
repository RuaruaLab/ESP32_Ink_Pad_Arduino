#include "EPD_2in13g.h"
#include "imagedata.h"

#ifndef EPAPER_DISPLAY_HPP
#define EPAPER_DISPLAY_HPP

#include "spleen-12x24.h"

extern uint8_t IMG_buffer[7750];


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

    void begin();
    void setPixel(int row, int col, PixelColor color);
    void clearScreen(PixelColor color);
    void refreshScreen();
    void set(const UBYTE *Image);
    void drawString(int startX, int startY, const char* str, PixelColor color);     //绘制字符串
    void sleep();
};

extern EPaperDisplay display;

#endif