# Configuration eth1
# RAPPEL: eth0 est à vagrant, ne pas y toucher

## Désactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False

## Suppression de la passerelle par défaut
ip route del default:
  cmd:
    - run

##Configuration de VM3
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:2::36
    - ipv6netmask: 64

eth2:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:4::36
    - ipv6netmask: 64

## Configuration de la route vers LAN2 via VM2
routes_eth1:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN1-6
        ipaddr: fc00:1234:1::/64
        gateway: fc00:1234:2::26
      - name: LAN3-6
        ipaddr: fc00:1234:3::/64
        gateway: fc00:1234:2::26

net.ipv6.conf.all.forwarding:
  sysctl:
    - present
    - value: 1

dhclient eth0:
  cmd:
    - run

radvd:
  pkg:
    - installed
  service:
    - running
    - enable: True

/etc/radvd.conf:
  file.managed:
    - source: /vagrant/radvd.conf
    - user: root
    - group: root
    - mode: 644

inetutils-inetd:
  pkg:
    - installed

update-inetd --add "echo stream tcp6 nowait nobody internal":
  cmd:
    - run

service inetutils-inetd start:
  cmd:
    - run

service inetutils-inetd restart:
  cmd:
    - run
