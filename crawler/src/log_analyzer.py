#!/usr/bin/env python3

import os

def main():
    nb_threads = 1000
    nb_success = 0

    for i in range(1, nb_threads + 1):
        try:
            f = open(f"logs/success.thread-{str(i)}", "r")
            data = f.read()
            f.close()

            nb_success += int(data.split(':')[1][1:])
        except:
            pass

    nb_errors = os.popen("cat logs/error* | wc -l").read()

    print("Number of certificates successfully added in database:", nb_success, "for a total of", nb_success + int(nb_errors))

if __name__ == "__main__":
    main()
