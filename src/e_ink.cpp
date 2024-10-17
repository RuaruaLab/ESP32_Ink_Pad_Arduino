#include "e_ink.hpp"

void INK_Display::refresh()
{
    epd.Display(IMG_buffer);
}

void INK_Display::img_pixel_set(int rol, int col, uint8_t color)
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
    IMG_buffer[byte_index] &= ~(0x03 << bit_offset);
    IMG_buffer[byte_index] |= (color << bit_offset);
}

void INK_Display::img_clear()
{
    /*缓冲区设置为全白*/
    int pixel_vel;
    for(int i=0; i<sizeof(IMG_buffer); i++){
        pixel_vel = PIXEL_WHITE;
        IMG_buffer[i] = pixel_vel << 6 | pixel_vel << 4 | pixel_vel << 2 | pixel_vel;
    }
}

INK_Display::INK_Display(/* args */)
{
    epd.Init();
    img_clear();
}

INK_Display::~INK_Display()
{
}