# Variant 6

Templates to support: **2, 3, 6, 7, 8, 9, 26, 28, 29**.

## Argument types

| Symbol      | Width   | Semantics                                                    |
|-------------|---------|--------------------------------------------------------------|
| `/reg`      | 4 bits  | Register number; printed as `R<n>` (decimal, `R0`–`R15`).    |
| `/addr`     | 32 bits | Virtual address; printed as `[0x........]` (hex, bracketed). |
| `/lit16`    | 16 bits | Signed integer; printed as signed decimal.                   |
| `/lit32`    | 32 bits | Signed integer; printed as signed decimal.                   |

## Instruction table

| #  | Mnemonic                       | Encoding                  | Example              | Total bytes | Opcode bits |
|----|--------------------------------|---------------------------|----------------------|-------------|-------------|
| 2  | `MOV <reg>, <addr>`            | `1B 0 /reg /addr`         | `1B 01 00000042`     | 6           | 12          |
| 3  | `MOV <addr>, <reg>`            | `1B 1 /reg /addr`         | `1B 11 00000042`     | 6           | 12          |
| 6  | `ADD <reg1>, <reg2>, <reg3>`   | `03 0 /reg1 /reg2 /reg3`  | `03 01 23`           | 3           | 12          |
| 7  | `ADD <reg1>, <reg2>, <addr>`   | `04 /reg1 /reg2 /addr`    | `04 12 00000042`     | 6           | 8           |
| 8  | `SUB <reg1>, <reg2>`           | `0A /reg1 /reg2`          | `0A 12`              | 2           | 8           |
| 9  | `SUB <reg>, <addr>`            | `0B 0 /reg /addr`         | `0B 01 00000042`     | 6           | 12          |
| 26 | `CMP <reg1>, <reg2>`           | `80 /reg1 /reg2`          | `80 12`              | 2           | 8           |
| 28 | `MOV <reg>, <lit16>`           | `1C 1 /reg /lit16`        | `1C 11 3344`         | 4           | 12          |
| 29 | `MOV <reg>, <lit32>`           | `1C 2 /reg /lit32`        | `1C 21 33445555`     | 6           | 12          |

## Semantics

| #  | Operation                                                                      |
|----|--------------------------------------------------------------------------------|
| 2  | Load value from memory at `<addr>` into register `<reg>`.                      |
| 3  | Store value from register `<reg>` into memory at `<addr>`.                     |
| 6  | `<reg1> = <reg2> + <reg3>`.                                                    |
| 7  | `<reg1> = <reg2> + mem32[<addr>]`.                                             |
| 8  | `<reg1> = <reg1> - <reg2>`.                                                    |
| 9  | `<reg> = <reg> - mem32[<addr>]`.                                               |
| 26 | Compare `<reg1>` and `<reg2>`; set flags (Intel-style).                        |
| 28 | Load 2-byte signed literal into `<reg>`.                                       |
| 29 | Load 4-byte signed literal into `<reg>`.                                       |