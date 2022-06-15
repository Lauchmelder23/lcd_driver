#include "interface.h"
#include "lcd.h"

#include <linux/gpio.h>
#include <linux/delay.h>

static void lcd_send_helper(const struct lcd_gpio_config* config, uint8_t data);

void lcd_power_on(const struct lcd_gpio_config* config)
{
    gpio_set_value(config->power, 1);
    gpio_set_value(config->rs, 0);
    gpio_set_value(config->rw, 0);
    gpio_set_value(config->enable, 0);

    gpio_set_value(config->power, 0);

    // 50ms power cycle
    mdelay(50);
    lcd_screen_init(config);
}

void lcd_power_off(const struct lcd_gpio_config* config)
{
    gpio_set_value(config->power, 1);
}

void lcd_send_command(const struct lcd_gpio_config* config, uint8_t command)
{
    gpio_set_value(config->rw, 0);
    gpio_set_value(config->rs, 0);

    lcd_send_helper(config, command);
}

void lcd_send_data(const struct lcd_gpio_config* config, uint8_t data)
{
    gpio_set_value(config->rw, 0);
    gpio_set_value(config->rs, 1);

    lcd_send_helper(config, data);
}

static void lcd_send_helper(const struct lcd_gpio_config* config, uint8_t data)
{
    gpio_set_value(config->enable, 1);

    gpio_set_value(config->data[3], (data >> 7) & 0x1);
    gpio_set_value(config->data[2], (data >> 6) & 0x1);
    gpio_set_value(config->data[1], (data >> 5) & 0x1);
    gpio_set_value(config->data[0], (data >> 4) & 0x1);

    udelay(1000);
    gpio_set_value(config->enable, 0);

    mdelay(10);
    gpio_set_value(config->enable, 1);

    gpio_set_value(config->data[3], (data >> 3) & 0x1);
    gpio_set_value(config->data[2], (data >> 2) & 0x1);
    gpio_set_value(config->data[1], (data >> 1) & 0x1);
    gpio_set_value(config->data[0], (data >> 0) & 0x1);

    udelay(1000);
    gpio_set_value(config->enable, 0);
    mdelay(10);
}

void lcd_screen_init(const struct lcd_gpio_config* config)
{
    lcd_send_command(config, (LCD_FUNC_SET | LCD_FOUR_BIT) >> 4);
    lcd_send_command(config, LCD_FUNC_SET | LCD_TWO_ROWS | LCD_FONT_5_7);
    lcd_send_command(config, LCD_DISPLAY_SWITCH | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_CURSOR_BLINK);
    lcd_send_command(config, LCD_SCREEN_CLEAR);
    lcd_send_command(config, LCD_INPUT_SET | LCD_LEFT_TO_RIGHT);
}