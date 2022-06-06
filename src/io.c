#ifndef LCD_IO_H_
#define LCD_IO_H_

#include "lcd.h"

loff_t lcd_llseek(struct file* filp, loff_t where, int whence)
{
    return 0;
}

ssize_t lcd_read(struct file* filp, char __user* buf, size_t count, loff_t* off)
{
    return 0;
}

ssize_t lcd_write(struct file* filp, const char __user* buf, size_t size, loff_t* off)
{
    return 0;
}

long lcd_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}

int lcd_open(struct inode* inode, struct file* file)
{
    return 0;
}

int lcd_release(struct inode* inode, struct file* file)
{
    return 0;
}

#endif