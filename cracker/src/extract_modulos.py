#!/usr/bin/env python3

from Crypto.PublicKey import RSA
import os

path = '../keys/'
files = os.listdir(path)

for filename in files:
    if not 'hex' in filename:
        filename = path + filename
        f = open(filename, "r")
        pubkey = RSA.importKey(f.read())
        n = pubkey.n
        h = hex(n)[2:] + '\n'
        g = open(filename + '.hex', 'w')
        g.write(hex(n)[2:] + '\n')
        g.close()
