#!/bin/bash
export CROSS_COMPILE=
export ARCH=x86_64
make -C /home/player999/Work/Ubuntu1404_linux313_source SUBDIRS=$PWD modules
