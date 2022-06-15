# LCD Driver
This is eventually gonna be a kernel module capable of sending text to a connected LCD display.
The communication would be happening via device file `/dev/lcd`

Installing the module creates the device `/dev/lcd` used for sending text to the screen. The LCD display
is turned on by opening the file for writing. A string of characters has to be ended with `\n` in order to
appear on the display.

This is the circuit I used to drive my screen:
![Circuit Diagram](./img/circuit.svg)

This is also the default pin layout used by the driver. If you want to use a different pin layout, you can change the driver's config as such:

    sudo ./install.sh pin_name=pin_number ...

where `pin_name` is one of the following:
 * `pin_power`
 * `pin_rs`
 * `pin_rw`
 * `pin_enable`
 * `pin_data`

and `pin_number` is the GPIO pin number. For `pin_data` you need to supply a comma-separated list of four numbers.

## How to build
Good luck.

## How to use
Probably better to just not use it.