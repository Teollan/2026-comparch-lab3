# Decompiler with virtual-address translation

A disassembler for a custom machine instruction set that also resolves virtual
addresses to physical ones. It reads a binary (or hex-encoded) byte stream,
decodes it according to the instruction encoding defined in [`docs/task.md`](docs/task.md),
translates any virtual address operands using memory tables supplied as CSV
files, and emits a human-readable assembly listing alongside the original bytes
of each instruction.

The supported instructions cover register-to-register and register-to-memory
data movement, arithmetic (`ADD`, `SUB`), comparison (`CMP`), and immediate
loads of 16- and 32-bit signed literals. See [`docs/task.md`](docs/task.md) for
the complete encoding and semantics tables.

## Virtual memory model

This build implements **variant 6**: a segment-paged organization with a 32-bit
virtual address split as follows.

```
 31           20 19         10 9            0
+--------------+-------------+--------------+
| segment (12) |  page (10)  |  offset (10) |
+--------------+-------------+--------------+
```

- **Page size:** 1 KB → 10-bit page offset.
- **Segment descriptor table size:** 4096 entries → 12-bit segment id.
- **Page number:** the remaining `32 - 10 - 12 = 10` bits.

A virtual address is translated by looking up the segment descriptor, then the
page within that segment's page table, then combining the resulting frame number
with the page offset: `physical = (frame << 10) | offset`.

When an address cannot be translated, an `ERROR:` line is emitted and analysis
of the remaining instructions continues. Translation can fail because:

- the segment id is not present in the segment table;
- the page is not in the segment's page table;
- the page is present in the table but not resident in main memory.

In the listing, the original (virtual) bytes are shown in the hex dump line,
while successfully translated instructions display the resolved physical address.
Instructions whose translation failed keep their original virtual address.

## Memory table files

Two kinds of CSV file describe the memory layout.

### Segment table

Each row describes one segment descriptor, in segment-id order:

```
<path-to-page-table.csv>,<page-count>
```

The path points to the page table for that segment. See
[`examples/example-segments.csv`](examples/example-segments.csv).

### Page table

Each row describes one page, in page-id order:

```
<page-number>,<present-flag>,<frame-number>
```

`present-flag` is `1` if the page is resident in main memory, `0` otherwise. See
[`examples/example-pages-0.csv`](examples/example-pages-0.csv).

## Building from source

### Requirements

- CMake 3.20 or newer
- A C++23-capable compiler (gcc/g++ 14.2 or newer)
- An internet connection on the first configure step (GoogleTest is fetched via `FetchContent`)

### Build

Using the provided `Makefile`:

```sh
make build
```

Or directly with CMake:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To use a different compiler, set `CXX` / `CC` (Makefile) or `CMAKE_CXX_COMPILER` / `CMAKE_C_COMPILER` (CMake) accordingly.

### Run the tests

```sh
make test
```

Or:

```sh
ctest --test-dir build --output-on-failure
```

## Running

After building, the executable is located at `build/app`.

```sh
./build/app --input <file> --segments-table <file>
```

The program reads bytes from `--input`, loads the memory tables starting from
`--segments-table`, disassembles the byte stream while translating virtual
addresses, and writes each instruction's hex bytes followed by its assembly form
to stdout (or to a file via `--output`).

### Examples

Disassemble a hex-encoded program using the example memory tables:

```sh
./build/app --hex --input examples/example-program.hex --segments-table examples/example-segments.csv
```

Write the listing to a file:

```sh
./build/app -x -i examples/example-program.hex -s examples/example-segments.csv -o out.asm
```

## Command-line arguments

| Long               | Short | Type   | Required | Description                                                                       |
|--------------------|-------|--------|----------|-----------------------------------------------------------------------------------|
| `--input`          | `-i`  | string | yes      | Path to the input file containing the byte stream to disassemble.                 |
| `--segments-table` | `-s`  | string | yes      | Path to the segment table CSV file describing the memory layout.                  |
| `--hex`            | `-x`  | flag   | no       | Treat the input as a hex-encoded text file instead of raw binary.                 |
| `--output`         | `-o`  | string | no       | Path to write the disassembly listing to. If omitted, output goes to stdout.      |

Arguments may be passed in any order. String arguments take the next token as their value (e.g. `--input file.bin`).
