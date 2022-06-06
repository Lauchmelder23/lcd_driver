#!/bin/bash

module=lcd
device=lcd

rmmod $module || exit 1
rm -rf /dev/$device
