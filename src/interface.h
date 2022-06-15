#ifndef _LCD_INTERFACE_H
#define _LCD_INTERFACE_H

#include <linux/types.h>

#define LCD_SCREEN_CLEAR    0x01
#define LCD_CURSOR_RETURN   0x02
#define LCD_INPUT_SET       0x04
#define LCD_DISPLAY_SWITCH  0x08
#define LCD_SHIFT_MODE      0x10
#define LCD_FUNC_SET        0x20

#define LCD_FOUR_BIT        0x00
#define LCD_EIGHT_BIT       0x10
#define LCD_ONE_ROW         0x00
#define LCD_TWO_ROWS        0x08
#define LCD_FONT_5_7        0x00
#define LCD_FONT_5_10       0x04

#define LCD_DISPLAY_OFF     0x00
#define LCD_DISPLAY_ON      0x04
#define LCD_CURSOR_OFF      0x00
#define LCD_CURSOR_ON       0x02
#define LCD_CURSOR_STATIC   0x00
#define LCD_CURSOR_BLINK    0x01

#define LCD_RIGHT_TO_LEFT   0x00
#define LCD_LEFT_TO_RIGHT   0x02
#define LCD_NO_SHIFT        0x00
#define LCD_SHIFT           0x01

struct lcd_gpio_config;

void lcd_power_on(const struct lcd_gpio_config* config);
void lcd_power_off(const struct lcd_gpio_config* config);

void lcd_send_command(const struct lcd_gpio_config* config, uint8_t command);
void lcd_send_data(const struct lcd_gpio_config* config, uint8_t data);

void lcd_screen_init(const struct lcd_gpio_config* config);

#endif