#!/bin/bash
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

mknod /dev/net/tun c 10 200
chmod 0666 /dev/net/tun
modprobe tun
ip -6 link set tun0 up
ip -6 addr add fc00:1234:ffff::1/64 dev tun0
