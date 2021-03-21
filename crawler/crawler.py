#!/usr/bin/env python3

import os

from src.crawl import *
from src.output import *
from src.args import *

def main():
    os.system('rm -f logs/error* logs/success*')
    args = parse_args()
    crawl(args)

if __name__ == "__main__":
    main()
