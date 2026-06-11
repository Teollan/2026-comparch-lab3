# Decompiler

A disassembler for a custom machine instruction set. It reads a binary (or hex-encoded) byte stream, decodes it according to the instruction encoding defined in [`docs/task.md`](docs/task.md), and emits a human-readable assembly listing alongside the original bytes of each instruction.

The supported instructions cover register-to-register and register-to-memory data movement, arithmetic (`ADD`, `SUB`), comparison (`CMP`), and immediate loads of 16- and 32-bit signed literals. See [`docs/task.md`](docs/task.md) for the complete encoding and semantics tables.

## Building from source

### Requirements

- CMake 3.20 or newer
- A C++23-capable compiler (defaults to `g++-14` / `gcc-14`)
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

After building, the executable is located at `build/apps/app`.

```sh
./build/apps/app --input <file>
```

The program reads bytes from `--input`, disassembles them, and writes each instruction's hex bytes followed by its assembly form to stdout (or to a file via `--output`).

### Examples

Disassemble a binary file:

```sh
./build/apps/app --input examples/complete.bin
```

Disassemble a hex-encoded text file:

```sh
./build/apps/app --input examples/complete.hex --hex
```

Write the listing to a file:

```sh
./build/apps/app -i examples/basic.hex -x -o out.asm
```

## Command-line arguments

| Long       | Short | Type   | Required | Description                                                                       |
|------------|-------|--------|----------|-----------------------------------------------------------------------------------|
| `--input`  | `-i`  | string | yes      | Path to the input file containing the byte stream to disassemble.                 |
| `--hex`    | `-x`  | flag   | no       | Treat the input as a hex-encoded text file instead of raw binary.                 |
| `--output` | `-o`  | string | no       | Path to write the disassembly listing to. If omitted, output goes to stdout.      |

Arguments may be passed in any order. String arguments take the next token as their value (e.g. `--input file.bin`).
