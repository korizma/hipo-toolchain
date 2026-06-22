# Format of the file after assembly

This file describes the output of the assembler.

## Structure of the Output File

First N bytes are is the String table. 

Afther the string table comes the Symbol table.

After the symbol table, the sections and their relocation tables.
If the section has a relocation table it is right after it.

```text

    String Table
    Symbol table
    Section
    *Relocation Table
    Section
    *Relocation Table
    ...
    Section
    *Relocation Table

```

## String table

First 8 bytes describe the number of strings in the string table.

Next are the strings which can be any size and are NULL terminated.

## Symbol table

First 8 bytes describe the number of entries in the Symbol table.

Then are N entries in the symbol table. Size of one entry is 40 bytes.
This is what an entry looks like

```text

index of the symbol in the string table - 8 bytes unsigned
symbol type - 4 bytes
symbol binding - 4 bytes
index of the section that the symbol belongs - 8 bytes unsigned
symbol offset - 8 bytes unsigned
symbol value - 8 bytes signed

```

## Section

A section is described by a header and its contents.
The size of the header is 17 bytes

This is what the headers contains

```text

the index of the sections name in the string table - 8 bytes
size of the section - 8 bytes
flag if the section has a rela table - 1 byte (0x00 means no rela table, 0x01 means it has a rela table)

```

After the header the next X bytes are the content of the section.

## Rela table

First 8 bytes describe the number of entries in the rela table.

After the 8 bytes are the entries.
The size of an entry is 28 bytes.

This is what an entry looks like.

```text
offset in the section to replace - 8 bytes unsigned
index of the symbol in the symbol table - 8 bytes unsigned
relocation type - 4 bytes
the addend - 8 bytes signed
```