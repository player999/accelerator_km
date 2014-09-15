#!/bin/bash

UUID=ac4e5a85-5e37-401f-8474-20d4316e4dee
DEVDISK=/dev/disk/by-uuid/$UUID
sudo mount $DEVDISK /mnt
sudo cp ./accelerator.ko /mnt/home/root
sudo cp ./userspace /mnt/home/root 
sudo umount /mnt

