#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/gpio.h>

#include "lcd.h"

MODULE_LICENSE("GPL");

int lcd_major = LCD_MAJOR;
int lcd_minor = LCD_MINOR;

module_param(lcd_major, int, S_IRUGO);
module_param(lcd_minor, int, S_IRUGO);

static struct lcd_dev lcddev = { 0 };
static int  __init lcd_init_cdev(void);
static void __exit lcd_exit_cdev(void);

static int  __init lcd_init_gpio(void);
static void __exit lcd_exit_gpio(void);

extern loff_t   lcd_llseek  (struct file*, loff_t, int);
extern ssize_t  lcd_read    (struct file*, char __user*, size_t, loff_t*);
extern ssize_t  lcd_write   (struct file*, const char __user*, size_t, loff_t*);
extern long     lcd_ioctl   (struct file*, unsigned int, unsigned long);
extern int      lcd_open    (struct inode*, struct file*);
extern int      lcd_release (struct inode*, struct file*);

static struct file_operations lcd_fops = 
{
    .owner          = THIS_MODULE,
    .llseek         = lcd_llseek,
    .read           = lcd_read,
    .write          = lcd_write,
    .unlocked_ioctl = lcd_ioctl,
    .open           = lcd_open,
    .release        = lcd_release
};

static int __init lcd_init_module(void)
{
    int result = 0;

    if(lcd_major)
    {
        lcddev.devno = MKDEV(lcd_major, lcd_minor);
        result = register_chrdev_region(lcddev.devno, 1, "lcd");
    }
    else
    {
        result = alloc_chrdev_region(&lcddev.devno, lcd_minor, 1, "lcd");
        lcd_major = MAJOR(lcddev.devno);
    }

    if(result < 0)
    {
        printk(KERN_ERR "%s: Failed to register character device\n", THIS_MODULE->name);
        return result;
    }

    result = lcd_init_cdev();
    if(result < 0)
        return result;

    result = lcd_init_gpio();
    if(result < 0)
        return result;

    printk(KERN_INFO "%s: Module loaded!\n", THIS_MODULE->name);
    return result;
}

static void __exit lcd_exit_module(void)
{
    lcd_exit_gpio();
    lcd_exit_cdev();
    unregister_chrdev_region(lcddev.devno, 1);

    printk(KERN_INFO "%s: Module removed!\n", THIS_MODULE->name);
}

module_init(lcd_init_module);
module_exit(lcd_exit_module);

static int __init lcd_init_cdev(void)
{
    int result;

    cdev_init(&lcddev.dev, &lcd_fops);
    lcddev.dev.owner = THIS_MODULE;
    lcddev.dev.ops = &lcd_fops;

    result = cdev_add(&lcddev.dev, lcddev.devno, 1);
    if(result < 0)
        printk(KERN_ERR "%s: Failed to add device\n", THIS_MODULE->name);

    return result;
}

static void __exit lcd_exit_cdev(void)
{
    cdev_del(&lcddev.dev);
}

static int  __init lcd_init_gpio(void)
{
    lcddev.gpio = 18;
    gpio_direction_output(lcddev.gpio, 0);
    return 0;
}

static void __exit lcd_exit_gpio(void)
{
    
}