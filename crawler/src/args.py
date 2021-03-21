#!/usr/bin/env python3

import argparse

def parse_args():
    """
    Return argparse structure from command line arguments
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose',
        help='print debug informations',
        action='store_true')

    group_crt = parser.add_argument_group('--crt_crawler (default)',
        'this will crawl 100 millions crt.sh certificates')
    group_crt.add_argument('--crt_crawler',
        help=argparse.SUPPRESS,
        action='store_true')
    group_crt.add_argument('--crt_n', type=int,
        help='precise the number of certificates to dump')
    group_crt.add_argument('--threads', type=int,
        help='threads number - default is 1')
    group_crt.add_argument('--machine', type=int,
        help='machine ID - default is 1')

    group_ip = parser.add_argument_group('--ip_crawler',
        'this will crawl all ip addresses from 0.0.0.0 to 256.256.256.256')
    group_ip.add_argument('--ip_crawler',
        help=argparse.SUPPRESS,
        action='store_true')
    group_ip.add_argument('--ip_start', type=str,
        help='start crawler at a given ip = X.X.X.X/Y')
    group_ip.add_argument('--ip_end', type=str,
        help='end crawler at a given ip = X.X.X.X (not included)')

    group_oneshot = parser.add_argument_group('oneshot mode',
        'those options are useful to dump a precise certificate')
    group_oneshot.add_argument('-i', '--ip', type=str,
        help='get certificate of a given ip')
    group_oneshot.add_argument('-d', '--domain', type=str,
        help='get certificate of a given domain')
    group_oneshot.add_argument('-c', '--crt', type=str,
        help='get certificate of a `crt.sh` id')

    args = parser.parse_args()
    return args
