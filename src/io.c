#ifndef LCD_IO_H_
#define LCD_IO_H_

#include <linux/fs.h>
#include <linux/gpio.h>
#include "lcd.h"

static bool advance_ptr(const char* base, ssize_t size, char** ptr);

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
    char* kern_buf = NULL;
    char* cmd_start = NULL;
    char* cmd_end = NULL;
    ssize_t retval = 0;

    if(down_interruptible(&dev->sem))
        return retval;

    kern_buf = (char*)kmalloc(count, GFP_KERNEL);
    if(kern_buf == NULL)
    {
        printk(KERN_ERR "%s: Failed to allocate buffer (%d bytes), aborting write.\n", THIS_MODULE->name, count);
        retval = -ENOMEM;
        goto out;
    }

    if(copy_from_user(kern_buf, buf, count))
    {
        printk(KERN_ERR "%s: Failed to copy data from user, aborting write.\n", THIS_MODULE->name);
        retval = -EFAULT;
        kfree(kern_buf);
        goto out;
    }

    cmd_start = kern_buf;
    cmd_end = kern_buf;

    while(advance_ptr(kern_buf, count, &cmd_end))
    {
        *cmd_end = '\0';

        if(strcmp(cmd_start, "0") == 0)
            gpio_set_value(dev->gpio, 0);
        else if(strcmp(cmd_start, "1") == 0)
            gpio_set_value(dev->gpio, 1);
        else
            printk(KERN_WARNING "%s: Unrecognized command \"%s\"\n", THIS_MODULE->name, cmd_start);

        cmd_start = cmd_end + 1;
    }

    retval = count;
    kfree(kern_buf);

out:
    up(&dev->sem);
    return retval;
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

    printk(KERN_DEBUG "%s: Opened device\n", THIS_MODULE->name);
    return 0;
}

int lcd_release(struct inode* inode, struct file* filp)
{
    struct lcd_dev* dev;
    dev = (struct lcd_dev*)filp->private_data;
    
    printk(KERN_DEBUG "%s: Released device\n", THIS_MODULE->name);
    return 0;
}

static bool advance_ptr(const char* base, ssize_t size, char** ptr)
{
    while(*ptr - base < size && **ptr != '\n')
        (*ptr)++;

    return *ptr - base < size;
}

#endif