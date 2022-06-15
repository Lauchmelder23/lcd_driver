#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/module.h>

#include "lcd.h"

int lcd_proc_open(struct inode* inode, struct file* filp)
{
    struct lcd_dev* dev = PDE_DATA(inode);
    filp->private_data = (void*)dev;

    return 0;
}

ssize_t lcd_proc_read(struct file* filp, char __user* buf, size_t count, off_t* offset)
{
    struct lcd_dev* dev = (struct lcd_dev*)filp->private_data;

    char* output;
    size_t len = 0;

    if(*offset > 0)
        return 0;

    if(count < 512)
        return 0;

    output = (char*)kmalloc(512, GFP_KERNEL);
    if(output == NULL)
        return 0;

    len += sprintf(output + len, "Device maj %u min %u\n", MAJOR(dev->devno), MINOR(dev->devno));
    len += sprintf(output + len, "=== PIN CONFIGURATION ===\n");

    len += sprintf(output + len, "Power : %d\n", dev->config.power);
    len += sprintf(output + len, "RS    : %d\n", dev->config.rs);
    len += sprintf(output + len, "R/W   : %d\n", dev->config.rw);
    len += sprintf(output + len, "Enable: %d\n", dev->config.enable);
    len += sprintf(output + len, "D4-D7 : [%d, %d, %d, %d]\n", dev->config.data[0],  dev->config.data[1],  dev->config.data[2],  dev->config.data[3]);

    if(copy_to_user(buf, output, len) != 0)
    {
        printk(KERN_WARNING "%s: Failed to copy some bytes to /proc\n", THIS_MODULE->name);
    }

    *offset += len;
    return len;
}

extern loff_t lcd_proc_lseek (struct file* filp, loff_t offset, int whence)
{
    return 0;
}

int lcd_proc_release(struct inode* inode, struct file* filp)
{
    return 0;
}