#!/bin/sh

module=lcd
device=lcd

/sbin/insmod ./$module.ko $* || exit 1

rm -rf /dev/$device

major=$(awk "\$2 == \"$module\" { print \$1 }" /proc/devices)
echo $major

mknod /dev/$device c $major 0
chmod 664 /dev/$device
