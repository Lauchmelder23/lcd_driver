#ifndef _LCD_CHECKS_H
#define _LCD_CHECKS_H

struct lcd_gpio_config;

int lcd_is_valid_gpio(int pin);
int lcd_assign_gpio(int pin);

int lcd_check_duplicates(const int* config);

#endif