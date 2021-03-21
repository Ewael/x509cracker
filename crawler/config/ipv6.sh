#!/bin/bash

if [ "$#" -ne 2 ]; then
    # cafe & /52
    echo "Usage: $0 LOCAL_IPV6 ROUTED_PREFIX"
    echo "! Please update the script before using it !"
    exit 1
fi

# change those 2 lines depending the OS
apt update
apt install -y wireguard wireguard-tools

# please change every `alice` references with your own config

PRIVATE_KEY=$(wg genkey)
PUBLIC_KEY=$(echo $PRIVATE_KEY | wg pubkey)
CONF="[Interface]
PrivateKey=$PRIVATE_KEY
Address=$1/128

# alice
[Peer]
PublicKey=[alice base64 pubkey]
Endpoint=[alice endpoint X.X.X.X:PORT]
PersistentKeepAlive=25
AllowedIPs=[ipv6 range xxxx:xxxx::/64]
AllowedIPs=::/0"

echo "$PUBLIC_KEY" > ./public_key
echo "[!] Don't forget to add the public key."
echo "[!] The public key is: $PUBLIC_KEY"

echo "$CONF" > /etc/wireguard/wg-alice.conf

echo "net.ipv6.ip_nonlocal_bind = 1" >> /etc/sysctl.conf
sysctl --system

systemctl enable wg-quick@wg-alice
systemctl start wg-quick@wg-alice

ip -6 route add local "$2" dev lo
