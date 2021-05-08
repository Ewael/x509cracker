# x509cracker

This is a one-month school laboratory projet from the [LSE](http://lse.epita.fr)
by first-year [EPITA](https://www.epita.fr) students of the security team. This
repository contains the source code of the project but we have no intention to
maintain it / to make it portable. Also please take a look at the PDF before
browsing the code to fully understand the implementation.

## Crawler

This is a multithreaded web crawler used to obtain a very large quantity of
x509 RSA certificates (100+ millions). The whole implementation is explained in
the project report. Here are only the basic steps we had to do to run the
crawler once we finished coding it:

- update the `config/ipv6.sh` file with your `wireguard` configuration
- run the script from the `config` directory with `sudo ./ipv6.sh LOCAL_IPV6 ROUTED_PREFIX`
- install the needed python libraries with `pip install -r requirements.txt`
- add your subnets at the top of `src/crawl.py`

To track the machines the crawler is running on:

- update `send_data.sh` with the machine ID and the url of the dashboard

## Cracker

This implementation of the cracker uses the batch-gcd algorithm as described in
the report. This is coded in pure C++ and you must follow those steps to run it
properly:

- create a `build` directory and use `cmake ..` so it is easy to clean up
- compile with `make -j 12`

You also need the following packages:
- `gtest`
- `gmock`
- `gmp`
- `gmpxx`
- `sqlite3`

To use the cracker with a database, create it with the correct format or using
the crawler appropriately.

```
./cracker --db <path to your db>
```

You can also use a directory containing modulos in hexadecimal format. The files
must have the `.hex` prefixes (see PoC).

```
./cracker --from-dir <directory containing .hex files>
```

## TODO

- Translate the report in english

---

![lse](lse.png)
