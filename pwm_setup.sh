#!/bin/bash

# set up pwm0 (pin 32)
sudo busybox devmem 0x700031fc 32 0x45
sudo busybox devmem 0x6000d504 32 0x2

# set up pwm2 (pin 33)
sudo busybox devmem 0x70003248 32 0x46
sudo busybox devmem 0x6000d100 32 0x00