#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>

#include "lcd.h"
#include "checks.h" 

MODULE_LICENSE("GPL");

static int lcd_major = LCD_MAJOR;
static int lcd_minor = LCD_MINOR;

module_param(lcd_major, int, S_IRUGO);
module_param(lcd_minor, int, S_IRUGO);

static int pin_power = LCD_PIN_POWER;
module_param(pin_power, int, S_IRUGO);

static int pin_rs = LCD_PIN_REGISTER_SELECT;
module_param(pin_rs, int, S_IRUGO);

static int pin_rw = LCD_PIN_READ_WRITE;
module_param(pin_rw, int, S_IRUGO);

static int pin_enable = LCD_PIN_ENABLE;
module_param(pin_enable, int, S_IRUGO);

static int pin_data[4] = LCD_PIN_DATA;
static int count;
module_param_array(pin_data, int, &count, S_IRUGO);

static struct lcd_dev lcddev = { 0 };
static int __init lcd_init_cdev(void);
static int __init lcd_init_gpio(void);
static int __init lcd_validate_gpio(void);

static void __exit lcd_exit_cdev(void);
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

extern int      lcd_proc_open       (struct inode*, struct file*);
extern ssize_t  lcd_proc_read       (struct file*, char __user*, size_t, loff_t*);
extern loff_t   lcd_proc_lseek      (struct file*, loff_t, int);
extern int      lcd_proc_release    (struct inode*, struct file*);

static struct proc_ops lcd_pops = 
{
    .proc_open      = lcd_proc_open,
    .proc_read      = lcd_proc_read,
    .proc_lseek     = lcd_proc_lseek,
    .proc_release   = lcd_proc_release
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

    sema_init(&lcddev.sem, 1);

    result = lcd_init_cdev();
    if(result < 0)
        return result;

    result = lcd_init_gpio();
    if(result < 0)
    {
        printk(KERN_ERR "%s: Failed to configure GPIO pins\n", THIS_MODULE->name);
        return result;
    }

    gpio_set_value(lcddev.config.power, 1);

    lcddev.proc_dir = proc_create_data("lcd", 0, NULL, &lcd_pops, (void*)&lcddev);

    printk(KERN_INFO "%s: Module loaded!\n", THIS_MODULE->name);
    return result;
}

static void __exit lcd_exit_module(void)
{
    proc_remove(lcddev.proc_dir);
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

static int  __init lcd_init_gpio(void)
{
    int result, i;
    
    result = lcd_validate_gpio();
    
    if(result < 0)
        return result;
    
    for(i = 0; i < 8; i++) {
        gpio_direction_output(lcddev.used_pins[i], 0);
    }

    return result;
}

static int __init lcd_validate_gpio(void)
{
    int result = 0;
    struct lcd_gpio_config* config = &lcddev.config;
    lcddev.used_pins = (int*)config;

    config->power   = lcd_assign_gpio(pin_power);
    config->rs      = lcd_assign_gpio(pin_rs);
    config->rw      = lcd_assign_gpio(pin_rw);
    config->enable  = lcd_assign_gpio(pin_enable);

    config->data[0] = lcd_assign_gpio(pin_data[0]);
    config->data[1] = lcd_assign_gpio(pin_data[1]);
    config->data[2] = lcd_assign_gpio(pin_data[2]);
    config->data[3] = lcd_assign_gpio(pin_data[3]);

    result = lcd_check_duplicates(lcddev.used_pins);
    if(result < 0)
    {
        printk(KERN_ERR "%s: Invalid pin configuration (duplicate pin usage)\n", THIS_MODULE->name);
        return result;
    }

    return 0;
}

static void __exit lcd_exit_cdev(void)
{
    cdev_del(&lcddev.dev);
}


static void __exit lcd_exit_gpio(void)
{

}