#ifndef LCD_IO_H_
#define LCD_IO_H_

#include <linux/fs.h>
#include <linux/gpio.h>
#include "lcd.h"
#include "interface.h"

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
    struct lcd_dev* dev = (struct lcd_dev*)filp->private_data;
    struct lcd_gpio_config* cfg = &dev->config;
    char* data = (char*)kmalloc(count, GFP_KERNEL);
    char* ptr;

    printk(KERN_DEBUG "%s: test %d\n", THIS_MODULE->name, count);
    
    if(copy_from_user((void*)data, (const void*)buf, count) != 0)
    {
        printk(KERN_WARNING "%s: Failed to copy some bytes from user.\n", THIS_MODULE->name);
        goto out;
    }

    for(ptr = data; *ptr != '\0' && *ptr != '\n'; ptr++)
    {
        lcd_send_data(cfg, (uint8_t)*ptr);
    }

out:
    kfree(data);
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

    if(down_trylock(&dev->sem))
        return -EACCES;

    filp->private_data = (void*)dev;

    lcd_power_on(&dev->config);

    printk(KERN_DEBUG "%s: Opened device\n", THIS_MODULE->name);
    return 0;
}

int lcd_release(struct inode* inode, struct file* filp)
{
    struct lcd_dev* dev;
    dev = (struct lcd_dev*)filp->private_data;

    lcd_power_off(&dev->config);

    up(&dev->sem);

    printk(KERN_DEBUG "%s: Released device\n", THIS_MODULE->name);
    return 0;
}

#endif