#ifndef LCD_IO_H_
#define LCD_IO_H_

#include <linux/fs.h>
#include "lcd.h"

loff_t lcd_llseek(struct file* filp, loff_t where, int whence)
{
    return 0;
}

ssize_t lcd_read(struct file* filp, char __user* buf, size_t count, loff_t* off)
{
    return count;
}

ssize_t lcd_write(struct file* filp, const char __user* buf, size_t count, loff_t* off)
{
    return count;
}

long lcd_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}

int lcd_open(struct inode* inode, struct file* filp)
{
    struct lcd_dev* dev;
    dev = container_of(inode->i_cdev, struct lcd_dev, dev);
    filp->private_data = (void*)dev;

    printk(KERN_INFO "%s: Opened device\n", THIS_MODULE->name);
    return 0;
}

int lcd_release(struct inode* inode, struct file* filp)
{
    printk(KERN_INFO "%s: Released device\n", THIS_MODULE->name);
    return 0;
}

#endif