#!/bin/bash
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

mknod /dev/net/tun c 10 200
chmod 0666 /dev/net/tun
modprobe tun
gcc tunalloc.c
./a.out tun0
