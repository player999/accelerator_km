#!/bin/bash
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm
make -C /home/player999/Work/disseratation/own_distro/linux-socfpga SUBDIRS=$PWD modules
${CROSS_COMPILE}gcc -o userspace ./userspace_test.c
