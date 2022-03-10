# GDB demo

This is a educational project for GDB, Makefiles, and Git. It contains a buggy implementation of a class named `Tokenizer`. This class uses multi-worker thread for reading strings from a deque. Later split them by white spaces, and push the "tokens" into vectors, and at the last stage, return those vectors. This implementation has a number of bugs. The Makefile is comparatively lesser buggy.

## Building and Running

```bash
make all
make alltests
./demo  # does nothing; exists to demo some Makefile stuff
./testTokenizer.out
```

Run the following to make sure that you're using a newer version of GCC. You can also add this to your `.bash_profile` to make this change automatically.

```bash
module load gcc/7.1.0
```

To run in GDB,

```bash
gdb ./testTokenizer.out
```

To obtain core dumps, run:

```bash
ulimit -c unlimited
```

If `testTokenizer.out` crashes, it should produce a core dump: a file named
`core` or something like `core.12345`. 

Load that core dump with:

```bash
gdb ./testTokenizer.out -c core.12345
```
