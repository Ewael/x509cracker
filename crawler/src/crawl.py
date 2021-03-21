#!/usr/bin/env python3

from threading import Thread
from asn1crypto import pem, x509
import ipaddress
import requests
import ssl
import time
from requests_toolbelt.adapters import source
from random import seed, getrandbits
from ipaddress import IPv6Network, IPv6Address
import os

from .output import *
from .database import *

# default values
DEBUG_MODE = False
DEFAULT_IP_START = '0.0.0.0/0'
DEFAULT_IP_END = '256.256.256.256'
DEFAULT_CRT_START = 0
DEFAULT_CRT_END = 4056891483
DEFAULT_CRT_N = 100000000
CRT_URL = "https://crt.sh/?d="
DEFAULT_THREADS = 1
DEFAULT_MACHINE_ID = 1

# ipv6 config - TO BE CONFIGURED
# SUBNET = 'XXXX:YYYY:ZZZZ::/52'
SUBNETS = ["XXXX:YYYY:ZZZZ:1000::/52",
           "XXXX:YYYY:ZZZZ:2000::/52", "XXXX:YYYY:ZZZZ:3000::/52",
           "XXXX:YYYY:ZZZZ:4000::/52", "XXXX:YYYY:ZZZZ:5000::/52",
           "XXXX:YYYY:ZZZZ:6000::/52", "XXXX:YYYY:ZZZZ:7000::/52",
           "XXXX:YYYY:ZZZZ:8000::/52", "XXXX:YYYY:ZZZZ:9000::/52",
           "XXXX:YYYY:ZZZZ:a000::/52", "XXXX:YYYY:ZZZZ:b000::/52",
           "XXXX:YYYY:ZZZZ:c000::/52", "XXXX:YYYY:ZZZZ:d000::/52",
           "XXXX:YYYY:ZZZZ:e000::/52", "XXXX:YYYY:ZZZZ:f000::/52"]
# NETWORK = IPv6Network(SUBNET)
IPs_LIST = {}

# ------------------------- NETWORK UTILS

def is_port_open(ip, port=443):
    """
    Check if the given port is open (default is https)
    """
    conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    conn.settimeout(0.1)
    res = conn.connect_ex((ip, port))
    is_open = not bool(res)
    if not is_open and DEBUG_MODE:
        log(f'Port {port} is closed for {ip}', mode='error')
    return is_open

def domain_from_ip(ip):
    """
    Perform a simple reverse DNS lookup
    """
    try:
        record = socket.gethostbyaddr(ip)
        return record[0]
    except:
        if DEBUG_MODE:
            log(f'{ip} does not have a PTR record', mode='error')
        return None

# ------------------------- CERT MANIPULATION

def pubkey_from_cert(cert):
    """
    Extract public key from certificate
    """
    try:
        pubkey = cert['tbs_certificate']['subject_public_key_info']
        pubkey = pubkey.native["public_key"]["modulus"]
        return pubkey
    except:
        if DEBUG_MODE:
            log(f'Certificate\'s public key is not a RSA key', mode='error')
        return None

def cert_from_domain(domain):
    """
    Get cert from domain
    """
    if DEBUG_MODE:
        log(f'Getting cert for {domain}')
    ctx = ssl.create_default_context()
    s = ctx.wrap_socket(socket.socket(), server_hostname=domain)
    try:
        s.connect((domain, 443))
    except:
        if DEBUG_MODE:
            log(f'Could not establish secured connection to {domain} port 443',
                mode='error')
        return None
    der = s.getpeercert(binary_form=True)
    cert = x509.Certificate.load(der)
    return cert

def cert_from_ip(ip):
    """
    Get cert from ip
    """
    if DEBUG_MODE:
        log(f'Scanning {ip}')
    if not is_port_open(ip):
        return None
    domain = domain_from_ip(ip)
    if not domain:
        return None
    if DEBUG_MODE:
        log(f'{ip} -> domain is {domain}')
    return cert_from_domain(domain)

def cert_from_crt(crt_id, req_session, src_address):
    """
    Get cert, pem and response code from crt id
    """
    if DEBUG_MODE:
        log(f'Getting cert for crt id {crt_id} with source IP ' + src_address)
    new_source = source.SourceAddressAdapter(src_address)
    url = CRT_URL + str(crt_id)
    req_session.mount('https://', new_source)
    response = req_session.get(url)
    txt = response.text # pem
    error = response.status_code
    try:
        der_bytes = pem.unarmor(txt.encode('utf-8'))[2]
        cert = x509.Certificate.load(der_bytes)
    except:
        if DEBUG_MODE:
            log(f'Could not get certificate for crt id {crt_id}: error {error}',
                mode='error')
        return None, None, None
    return cert, txt, response.status_code

# ------------------------- MULTITHREADED CRAWLER

def change_ip(src=None):
    """
    Get a new unused IP from the subnet
    """
    seed()
    while True:
        address = IPv6Address(NETWORK.network_address + \
                getrandbits(NETWORK.max_prefixlen - \
                NETWORK.prefixlen))
        if address not in IPs_LIST:
            if src is not None and (src in IPs_LIST):
                IPs_LIST.pop(src)
            IPs_LIST[address] = True
            break
    return str(address)

def init_db(id_db):
    """
    Init the databse "manually"
    """
    filename = "cert_x509-" + str(id_db) + ".db"
    os.system("cp database/cert_x509.db ./data/" + filename)

def crawl_multithread(nb_threads, start, end, crt_n):
    """
    Crawl crt.sh certificates using threads
    """
    if nb_threads > crt_n:
        nb_threads = crt_n

    # init multithread params
    threads = [None] * nb_threads
    certs_by_thread = crt_n // nb_threads
    os.system("mkdir ./data")

    # multithreaded crawler
    for i in range(nb_threads):
        init_db(i)
        src_address = change_ip(None) # initial IP
        crt_start = i * certs_by_thread + start
        if i + 1 == nb_threads:
            crt_end = DEFAULT_CRT_END
            threads[i] = Thread(target=crt_crawl, \
                    args=(crt_start, crt_end, certs_by_thread, src_address, i))
        else:
            crt_end = crt_start + certs_by_thread + start
            threads[i] = Thread(target=crt_crawl, \
                    args=(crt_start, crt_end, certs_by_thread, src_address, i))
        threads[i].start()

    # close threads
    for i in range(nb_threads):
        threads[i].join()

# ------------------------- CLASSIC CRAWLERS

def crt_crawl(crt_start, crt_end, crt_n, src_address, id_thread):
    """
    Crawl certificates on crt.sh
    """
    crt_step = (crt_end - crt_start + 1) // crt_n # linear timeline
    req_session = requests.Session()
    counter = 0

    for crt_id in range(crt_start, crt_end, crt_step):
        try:
            cert, pem, status_code = cert_from_crt(crt_id, req_session, src_address)
        except Exception as e:
            log_fail_id(crt_id, id_thread, e)
            continue
        if status_code == 429:
            src_address = change_ip(src_address)
            log_fail_id(crt_id, id_thread, "error 429 - too many requests")
            continue
        if counter == 10:
            src_address = change_ip(src_address)
            counter = 0
        if not cert:
            log_fail_id(crt_id, id_thread, "could not get certificate")
            continue
        pubkey = pubkey_from_cert(cert)
        if not pubkey:
            log_fail_id(crt_id, id_thread, "could not get pubkey - not a RSA key")
            continue
        if DEBUG_MODE:
            log(f'Successfully retrieved certificate for crt id {crt_id}.',
                mode='success')
        log_success_id(id_thread, (crt_id - crt_start) // crt_step)
        not_before = str(cert['tbs_certificate']['validity']['not_before'].native).split()[0]
        not_after = str(cert['tbs_certificate']['validity']['not_after'].native).split()[0]
        try:
            if not cert.valid_domains or len(cert.valid_domains) == 0:
                values = (crt_id, "NULL", not_before, not_after, str(pubkey), pem)
            else:
                values = (crt_id, cert.valid_domains[0], not_before, not_after, str(pubkey), pem)
        except Exception as e:
            print("An error happened when processing cert", crt_id, e)
        insert_certificate(values, id_thread) # <----- actual insertion in database
        if DEBUG_MODE:
            log_certif(cert, pubkey)
        counter += 1

def ip_crawl(ip_start, ip_end):
    """
    Crawl on all IPv4 addresses to extract
    x509 certificates and their public keys
    """
    net4 = ipaddress.ip_network(ip_start)
    if DEBUG_MODE:
        log(f'Crawler starting at {ip_start}')
    for host in net4.hosts():
        ip = str(host)
        if ip == ip_end:
            break
        cert = cert_from_ip(ip)
        if not cert: # could not get certificate from ip
            continue
        pubkey = pubkey_from_cert(cert)
        if not pubkey:
            continue
        if DEBUG_MODE:
            log(f'Successfully retrieved certificate at {ip}', mode='success')
            log_certif(cert, pubkey)
    if DEBUG_MODE:
        log('Crawling over between {ip_start} and {ip_end}')

# ------------------------- MAIN

def crawl(args):
    """
    Redirect to the correct crawler depending on given options
    """
    global DEBUG_MODE, NETWORK, SUBNET
    DEBUG_MODE = args.verbose
    s = requests.Session()
    s.trust_env = False

    # oneshot mode
    if args.ip or args.domain or args.crt:
        cert = None
        if args.ip:
            cert = cert_from_ip(args.ip)
        elif args.domain:
            cert = cert_from_domain(args.domain)
        else:
            cert = cert_from_crt(args.crt)
        if not cert:
            log(f'Could not get certificate', mode='error')
            return
        pubkey = pubkey_from_cert(cert)
        if not pubkey:
            log(f'Could not get certificate', mode='error')
            return
        log_certif([cert], [pubkey], 1)

    # ip crawler
    elif args.ip_crawler:
        ip_start = args.ip_start if args.ip_start else DEFAULT_IP_START
        ip_end = args.ip_end if args.ip_end else DEFAULT_IP_END
        ip_crawl(ip_start, ip_end)

    # crt crawler
    else:
        crt_n = args.crt_n if args.crt_n else DEFAULT_CRT_N
        nb_threads = args.threads if args.threads else DEFAULT_THREADS
        machine_id = args.machine if args.machine and args.machine > 0 and args.machine <= 16 else DEFAULT_MACHINE_ID

        offset = 25000000
        crt_start = offset * (machine_id - 1)
        crt_end = crt_start + offset

        SUBNET = SUBNETS[machine_id - 1]
        NETWORK = IPv6Network(SUBNET)
        if nb_threads > 1:
            crawl_multithread(nb_threads, crt_start, crt_end, crt_n)
        else:
            crt_crawl(crt_start, crt_end, crt_n, change_ip(None))
