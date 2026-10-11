# c-hack-assembler

A Hack assembler written in C

A mostly literal translation from my implementation in Python:  
https://github.com/NickleDave/py-hack-assembler

I did look at the following implementations:
- https://codeberg.org/virtualfuzz/Hack_Assembler
- https://github.com/ianmurfinxyz/hackass_hack_assembler_c
- https://codeberg.org/zakuArbor/hackAssembler
- https://github.com/sahillathwal/c-hack-assembler

## Usage

To compile Hack assembly to binary, run:
```
Assembler Add.asm
```

By default, this will create a file with the same base name and the `.Hack` extension,
that contains the binary program as strings.

## Set-up

The build is done with Autotools.

For the initial set-up to build, run the following in the project root:

```
autoreconf --install
```

Running tests requires [Criterion](https://criterion.readthedocs.io/en/master/).
```
sudo apt-get install libcriterion-dev
```

## Build

To build (after setting up to build by running `autoreconf --install`), do the following:
```
cd build
../configure
make
```

This compiles `./build/src/Assembler`

To build the tests, do:
```
make check
```