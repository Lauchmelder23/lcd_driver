#ifndef _LCD_H
#define _LCD_H

#include <linux/module.h>
#include <linux/cdev.h>

#define LCD_MAJOR 0
#define LCD_MINOR 0

#define LCD_GPIO 18

struct lcd_dev
{
    dev_t devno;
    struct cdev dev;
    struct semaphore sem;
    int gpio;
};

#endif