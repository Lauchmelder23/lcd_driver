#ifndef _LCD_H
#define _LCD_H

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/proc_fs.h>

#define LCD_MAJOR 0
#define LCD_MINOR 0

#define LCD_PIN_POWER           14
#define LCD_PIN_REGISTER_SELECT 22
#define LCD_PIN_READ_WRITE      27
#define LCD_PIN_ENABLE          17
#define LCD_PIN_DATA            { 26, 19, 13, 6 }

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
    struct proc_dir_entry* proc_dir;
};

#endif