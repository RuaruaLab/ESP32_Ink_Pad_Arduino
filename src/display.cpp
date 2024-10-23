#include "display.hpp"

uint8_t IMG_buffer[7750];            //图片缓冲区
EPaperDisplay display(IMG_buffer);   //显示控制类


void EPaperDisplay::drawString(int startX, int startY, const char* str, PixelColor color)
{
    const int charWidth = 12;  // 字符的宽度
    const int charHeight = 24; // 字符的高度
    int x = startX;
    int y = startY;

    while (*str) {
        if (*str >= 0 && *str <= 255) {
            const uint8_t (*charBitmap)[12] = font_pointers[(uint8_t)*str]; // 获取字符位图
            if (charBitmap != NULL) {
                for (int row = 0; row < charHeight; row++) {
                    for (int col = 0; col < charWidth; col++) {
                        if (charBitmap[row][col]) {  // 如果位图中该位为1，则绘制像素
                            setPixel(y + charHeight - 1 - row, x + col, color); // 翻转行的顺序
                        }
                    }
                }
            }
            x += charWidth;  // 移动到下一个字符的起始位置
        }
        str++;  // 移动到下一个字符
    }
}

void EPaperDisplay::begin()
{
    memset(img_buffer, 0xFF, sizeof(IMG_buffer)); // 初始化为全白
    if (epd.Init() != 0) {
        Serial.begin(9600);
        Serial.println("e-Paper init failed!");
    } else {
        Serial.begin(9600);
        Serial.println("e-Paper init ok");
    }
}

void EPaperDisplay::setPixel(int row, int col, PixelColor color) 
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return; // 越界检查
    }
    int index = row + col * ROWS;
    int byte_index = index / 4;
    int bit_offset = 6 - (index % 4) * 2;

    img_buffer[byte_index] &= ~(0x03 << bit_offset);
    img_buffer[byte_index] |= (color << bit_offset);
}

void EPaperDisplay::clearScreen(PixelColor color)
{
    uint8_t packedColor = (color << 6) | (color << 4) | (color << 2) | color;
    for (int i = 0; i < sizeof(IMG_buffer); i++) {
        img_buffer[i] = packedColor;
    }
}

 //刷新成缓冲区内的图像
void EPaperDisplay::refreshScreen() 
{
    epd.Display(img_buffer);
}

//直接通过图像刷新屏幕
void EPaperDisplay::set(const UBYTE *Image){
    epd.Display(Image);
}

void EPaperDisplay::sleep() {
    epd.Sleep();
}