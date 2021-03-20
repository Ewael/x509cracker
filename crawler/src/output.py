#!/usr/bin/env python3

from colorama import Fore

# colors
RED = Fore.RED
GREEN = Fore.GREEN
BLUE = Fore.BLUE
RESET = Fore.RESET

def log(msg, mode='debug', thread=0):
    """
    Log informations according to mode:
    - debug
    - error
    - success
    """
    if mode == 'debug':
        print(f'[{BLUE}*{RESET}] {msg}')
    elif mode == 'error':
        print(f'[{RED}x{RESET}] {msg}')
    elif mode == 'success':
        print(f'[{GREEN}+{RESET}] {msg}')

def log_certif(cert, pubkey):
    """
    Pretty print for one certificate
    """
    log(f'{cert.valid_domains} : {pubkey}')

def log_certifs(certs, pubkeys, nb_certs):
    """
    Pretty print for all certificates
    """
    assert len(pubkeys) == nb_certs
    log('Domain : Public key')
    for i in range(nb_certs):
        log_certif(certs[i], pubkeys[i]);

def log_fail_id(crt_id, thread_id, error_msg='no details'):
    f = open(f"logs/error.thread-{str(thread_id)}", "a")
    f.write(f"crt_id = {str(crt_id)} : {error_msg}\n")
    f.close()

def log_success_id(thread_id, nb_cert):
    f = open(f"logs/success.thread-{str(thread_id)}", "w")
    f.write(f"Number of certificates added in database: {nb_cert}\n")
    f.close()
