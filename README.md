# nconv

This project implements a command line utility that converts 128-bit, positive
integers to/from decimal, hexadecimal, octal, and binary.

### Program Usage

The `nconv` tool's usage is as follows:

```bash
nconv [OPTION]... SRC_BASE TGT_BASE NUM
```

`SRC_BASE` and `TGT_BASE` tell the number base used in the source number, `NUM`,
and the desired base for the output. `SRC_BASE` and `TGT_BASE` can be anyone of
the supported base strings: `bin`, `dec`, `oct`, or `hex`.

There are additional options for specifying the width of the output value as
well as how to group the digits. Run `nconv --help` for more details.

Below are few `nconv` demo commands:

```bash
nconv hex dec 0xDEADBEEF --> 3735928559
nconv dec bin 3735928559 --> 11011110101011011011111011101111
nconv dec oct 3735928559 --> 33653337357
nconv -g 4 dec hex 3735928559 --> DEAD BEEF
nconv -g 4 -w 12 dec hex 3735928559 --> 0000 DEAD BEEF
```
