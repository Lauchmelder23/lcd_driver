#ifndef _LCD_H
#define _LCD_H

#include <linux/cdev.h>

#define LCD_MAJOR 0
#define LCD_MINOR 0

extern int lcd_major;
extern int lcd_minor;

struct lcd_dev
{
    dev_t devno;
    struct cdev dev;
};

#endif