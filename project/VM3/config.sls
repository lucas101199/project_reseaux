# Configuration eth1
# RAPPEL: eth0 est à vagrant, ne pas y toucher

## Désactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False

## Configuration de VM3
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.163
    - netmask: 28

eth2:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:4::3
    - ipv6netmask: 64

## Configuration de la route de VM3
routes_eth1:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN1
        ipaddr: 172.16.2.128/28
        gateway: 172.16.2.162

routes_eth2:
  network.routes:
    - name: eth2
    - routes:
      - name: LAN2-6
        ipaddr: fc00:1234:2::/64
        gateway: fc00:1234:4::36
      - name: LAN1-6
        ipaddr: fc00:1234:1::/64
        gateway: fc00:1234:4::36


## Installation de inetutils-inetd
inetutils-inetd:
  pkg:
    - installed
  service:
    - runnning
    - enable: True

update-inetd --add "echo stream tcp6 nowait nobody internal":
  cmd:
    - run

service inetutils-inetd restart:
  cmd:
    - run

## Installation de netcat6
netcat6:
  pkg:
    - installed
    
## Suppression de la passerelle par défaut
ip route del default:
  cmd:
    - run
