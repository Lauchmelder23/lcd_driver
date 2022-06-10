#ifndef _LCD_H
#define _LCD_H

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>

#define LCD_MAJOR 0
#define LCD_MINOR 0

#define LCD_PIN_POWER           14
#define LCD_PIN_REGISTER_SELECT 11
#define LCD_PIN_READ_WRITE      9
#define LCD_PIN_ENABLE          10
#define LCD_PIN_DATA            { 26, 19, 13, 06 }

struct lcd_gpio_config
{
    int power;
    int rs, rw, enable;
    int data[4];
} __attribute__((packed));

struct lcd_dev
{
    dev_t devno;
    struct cdev dev;
    struct semaphore sem;
    struct lcd_gpio_config config;
    int* used_pins;
};

#endif