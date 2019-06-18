# Linux thread memory usage

An experiment to show how much memory a new POSIX thread requires by itself.

The answer for me was **about 16 KiB** on a 64-bit Linux computer.

This is because actual physical memory will only be allocated for a thread when
it actually requires it, that is, when it triggers a page fault.

See [this great answer](https://unix.stackexchange.com/a/280865/147737) for
more details.

## How to run

```bash
clang-format -i main.c
gcc -O2 -Wall -Wextra -Wno-unused-parameter -pthread main.c -o main
./main
```

```bash
> 10000 threads. Using 165261312 B (157.6 MiB).
> 16.1 KiB per thread.
```
