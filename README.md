# Decimal Hexadecimal Binary (DHB) Converter

This project implements a command line utility that converts arbitrarily large,
positive integers to/from decimal, hexadecimal, and binary.

### Building

To build the project locally, you will need the following libraries and tools
installed:

* CMake3.13+
* C++ compiler supporting C++20 features
* GNU MP Developer Libraries

To build the project, change directory to the `scripts/` directory and run
`build.sh`

```bash
./build.sh
```
> **Note**
> `build.sh` supports additional option flags for building the project docs
> (requires [Doxygen][1]) and unit tests. Run `build.sh -h` to see all the
> options.

After a successful build, you will find the binary installed to `dhb/bin/dhb`.

### Program Usage

`dhb` tool usage is as follows:

```bash
dhb [OPTION]... SRC_BASE TGT_BASE NUM
```

`SRC_BASE` and `TGT_BASE` tell the number base used in the source number, `NUM`,
and the desired base for the output. `SRC_BASE` and `TGT_BASE` can be anyone of
the supported base strings: `bin`, `dec`, or `hex`.

There are additional options for specifying the width of the output value as
well as how to group the digits. Run `dhb --help` for more details.

Below are few `dhb` demo commands:

```bash
dhb hex dec 0xDEADBEEF --> 3735928559
dhb dec bin 42 --> 101010
dhb --grouping 4 dec hex 3735928559 --> DEAD BEEF
dhb --grouping 4 --width 12 dec hex 3735928559 --> 0000 DEAD BEEF
```

### Running the Tests

`dhb` has been unit tested using the GoogleTest framework in conjunction with
`ctest`. Be sure to run submodule init/update or clone the repo using the
`--recursive` flag to pull down GoogleTest sources.

To build the unit tests, run the build script as before with the addition of the
`-t` flag:

```bash
./build.sh -t
```

To run the test, change directory to `dhb/build` and run `ctest`.

[1]: https://www.doxygen.nl/
