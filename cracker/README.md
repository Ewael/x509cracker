# Cracker

## Compiling

You will need `gmp` and `gmpxx` packages, but also `sqlite3` to compile the project.

Run
````bash
mkdir build && cd build
cmake ..
make -j
```

## Usage

You can use the cracker with a sqlite database **only if** your modulos are in a hex format
inside a table named *certificates* in a column named *modulus*.

If you respect these requirements, run:
```
./cracker --db <path to your db>
```

A simpler way is to put your keys in a hex format, one key per file in a directory.
These files must end with the suffix `.hex`.
Then run:
```
./cracker --from-dir <directory containing .hex files>
```
