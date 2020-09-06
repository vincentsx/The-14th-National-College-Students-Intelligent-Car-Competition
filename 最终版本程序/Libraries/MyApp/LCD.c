#include "headfile.h"


uint16 tft_x=TFT_X_MAX;
uint16 tft_y=TFT_Y_MAX;

uint8   st7735s_dir = LCD_DIR;
void LCD_dir(uint8 option)
{
//    ASSERT(option < 4);

    st7735s_dir = option;
    switch(option)
    {
    case 0:
        Lcd_WriteIndex(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        Lcd_WriteData(0xc8);//0xc8  0xA8 0x08 0x68

        Lcd_WriteIndex(0x2a);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x02);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x81);

        Lcd_WriteIndex(0x2B);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x03);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x82);

        tft_x   = TFT_X_MAX;
        tft_y   = TFT_Y_MAX;
        break;
    case 1:
        Lcd_WriteIndex(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        Lcd_WriteData(0xA8);//0xc8  0xA8 0x08 0x68

        Lcd_WriteIndex(0x2a);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x03);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x82);

        Lcd_WriteIndex(0x2B);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x02);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x81);

        tft_x   = TFT_Y_MAX;
        tft_y   = TFT_X_MAX;
        break;
    case 2:
        Lcd_WriteIndex(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        Lcd_WriteData(0x08);//0xc8  0xA8 0x08 0x68

        Lcd_WriteIndex(0x2a);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x02);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x81);

        Lcd_WriteIndex(0x2B);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x03);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x82);

        tft_x   = TFT_X_MAX;
        tft_y   = TFT_Y_MAX;
        break;
    case 3:
        Lcd_WriteIndex(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        Lcd_WriteData(0x68);//0xc8  0xA8 0x08 0x68

        Lcd_WriteIndex(0x2a);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x03);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x82);

        Lcd_WriteIndex(0x2B);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x02);
        Lcd_WriteData(0x00);
        Lcd_WriteData(0x81);

        tft_x   = TFT_Y_MAX;
        tft_y   = TFT_X_MAX;
        break;
    default:
        break;
    }

}