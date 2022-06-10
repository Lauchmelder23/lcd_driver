#include "checks.h"

#include <linux/module.h>
#include "lcd.h"


#define LCD_MIN_GPIO 2
#define LCD_MAX_GPIO 27

static int sub_pin = LCD_MIN_GPIO;

int lcd_is_valid_gpio(int pin)
{
    return (LCD_MIN_GPIO <= pin && pin <= LCD_MAX_GPIO);
}

int lcd_assign_gpio(int pin)
{
    if(lcd_is_valid_gpio(pin))
        return pin;


    printk(KERN_WARNING "%s: GPIO pin %d is invalid. Using %d instead\n", THIS_MODULE->name, pin, sub_pin);
    return sub_pin++;
}

int lcd_check_duplicates(const int* used_pins)
{
    int i, j;

    for(i = 0; i < 7; i++)
        for(j = i + 1; j < 8; j++)
            if(used_pins[i] == used_pins[j])
                return -EFAULT;

    return 0;
}