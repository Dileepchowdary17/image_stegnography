# LSB Image Steganography

A command-line based **LSB Image Steganography application developed in C** for hiding and extracting secret files inside BMP images. The application uses **Least Significant Bit (LSB) manipulation** to embed binary data into image bytes while preserving the visual appearance of the carrier image.

The project supports hiding different types of binary files such as **PDF, MP3, MP4, PNG, JPEG, and TXT** files. It implements separate encoding and decoding operations with metadata handling, magic-string verification, image capacity validation, binary file processing, bitwise operations, and command-line arguments.

---

## Features

**LSB-Based Data Hiding** — Hide secret file data inside the least significant bits of BMP image bytes.

**Binary File Support** — Encode and decode different file types such as PDF, MP3, MP4, PNG, JPEG, and TXT.

**Magic String Verification** — Use a user-defined magic string to identify and verify the encoded stego image before decoding.

**Magic String Size Encoding** — Store the size of the magic string inside the image so that the decoder knows how many characters to extract.

**File Extension Encoding** — Store the secret file extension inside the stego image so that the recovered file can be created with the correct extension.

**Secret File Size Encoding** — Store the original secret file size to determine exactly how many bytes must be decoded.

**BMP Header Preservation** — Copy the original BMP header without modification before embedding the secret information.

**Image Capacity Validation** — Calculate whether the source BMP contains sufficient capacity to store the complete payload before encoding.

**Binary File Handling** — Process secret files using binary file operations to preserve the original byte data.

**Command-Line Interface** — Perform encoding and decoding operations directly through terminal commands.

**Metadata-Based Decoding** — Decode the stored metadata in a predefined order before extracting the actual secret file.

**Byte-Level Processing** — Perform encoding and decoding using byte-level and bitwise operations.

---

## Project Structure

```text
LSB_Image_Steganography/
│
├── main.c              # Program entry point and command-line argument handling
├── encode.c            # Secret data encoding implementation
├── encode.h            # Encoding module declarations and EncodeInfo structure
├── decode.c            # Secret data decoding implementation
├── decode.h            # Decoding module declarations and DecodeInfo structure
├── types.h             # Common data types, status codes, and operation types
├── README.md           # Project documentation
└── sample files         # BMP and secret files used for testing
```

---

## Requirements

- GCC or any standard C compiler
- Linux, macOS, or Windows
- Terminal or Command Prompt
- Basic C development environment
- BMP image for use as the carrier file
- Secret file to encode

No external libraries or third-party dependencies are required.

---

## File Description

| File | Description |
|------|-------------|
| `main.c` | Program entry point. Handles command-line arguments and selects encoding or decoding operations. |
| `encode.c` | Implements BMP validation, file handling, capacity checking, metadata encoding, secret file encoding, and stego image generation. |
| `encode.h` | Contains the `EncodeInfo` structure and function declarations required by the encoding module. |
| `decode.c` | Implements metadata decoding, magic-string verification, secret file reconstruction, and output file generation. |
| `decode.h` | Contains the `DecodeInfo` structure and function declarations required by the decoding module. |
| `types.h` | Defines common status values, operation types, and shared data types used throughout the project. |
| `README.md` | Contains project documentation, implementation details, usage instructions, and examples. |

---

# Application Flow

The application provides two primary operations:

```text
                    LSB IMAGE STEGANOGRAPHY
                              |
                +-------------+-------------+
                |                           |
             ENCODING                   DECODING
                |                           |
          Source BMP                    Stego BMP
                |                           |
          Secret File                 Hidden Data
                |                           |
                v                           v
        Validate Inputs             Validate Inputs
                |                           |
                v                           v
        Open Files                  Open Stego Image
                |                           |
                v                           v
       Calculate File Size          Skip BMP Header
                |                           |
                v                           v
       Check Image Capacity         Decode Magic Size
                |                           |
                v                           v
        Copy BMP Header             Decode Magic String
                |                           |
                v                           v
       Encode Metadata              Verify Magic String
                |                           |
                v                           v
       Encode Secret Data            Decode Extension
                |                           |
                v                           v
       Copy Remaining Data           Decode File Size
                |                           |
                v                           v
          stego.bmp                 Decode File Data
                                            |
                                            v
                                      Recovered File
```

---

# Encoding Process

The encoding operation hides the secret file inside the BMP image.

The command structure is:

```text
./a.out -e source.bmp secret_file stego.bmp
```

The encoder follows a predefined data layout:

```text
+-----------------------------+
| BMP Header                  |
+-----------------------------+
| Magic String Size           |
+-----------------------------+
| Magic String                |
+-----------------------------+
| Secret Extension Size       |
+-----------------------------+
| Secret Extension            |
+-----------------------------+
| Secret File Size            |
+-----------------------------+
| Secret File Data            |
+-----------------------------+
| Remaining Image Data        |
+-----------------------------+
```

---

## Encoding Workflow

```text
Start
  |
  v
Read Command-Line Arguments
  |
  v
Check Encode Operation (-e)
  |
  v
Validate Source BMP
  |
  v
Validate Secret File
  |
  v
Open Source Image
  |
  v
Open Secret File
  |
  v
Create Stego Image
  |
  v
Read Magic String
  |
  v
Extract Secret File Extension
  |
  v
Calculate Secret File Size
  |
  v
Check Image Capacity
  |
  v
Copy BMP Header
  |
  v
Encode Magic String Size
  |
  v
Encode Magic String
  |
  v
Encode Secret File Extension Size
  |
  v
Encode Secret File Extension
  |
  v
Encode Secret File Size
  |
  v
Encode Secret File Data
  |
  v
Copy Remaining Image Data
  |
  v
Close Files
  |
  v
Stego Image Generated
  |
  v
End
```

---

## 1. Input Validation

The application first validates the command-line arguments.

Example:

```text
./a.out -e source.bmp secret.mp3 stego.bmp
```

The encoder validates:

- Operation type
- Source BMP image
- Secret file
- Output BMP file

The secret file is treated as binary data, so the encoding logic is not restricted to text files.

---

## 2. Open Files

The encoder opens three files:

```text
Source BMP       → Binary Read Mode
Secret File      → Binary Read Mode
Stego Image      → Binary Write Mode
```

Conceptually:

```c
fopen(source_image, "rb");
fopen(secret_file, "rb");
fopen(stego_image, "wb");
```

Binary mode is important because files such as PDF, MP3, MP4, PNG, and JPEG contain arbitrary byte values.

---

## 3. Read Magic String

The user provides a magic string during encoding.

Example:

```text
Enter your magic string:
MYSECRET
```

The magic string is embedded into the BMP image along with its size.

The magic string is later used during decoding to verify that the expected hidden data is present.

---

## 4. Extract Secret File Extension

The encoder extracts the extension from the secret file.

Example:

```text
secret.mp3
```

produces:

```text
.mp3
```

The extension is stored inside the image so that the decoder can reconstruct the correct output file type.

Examples:

```text
document.pdf  → .pdf
song.mp3      → .mp3
video.mp4     → .mp4
image.png     → .png
photo.jpeg    → .jpeg
data.txt      → .txt
```

---

## 5. Calculate Secret File Size

The encoder determines the size of the secret file in bytes.

For example:

```text
secret.mp3
```

may have:

```text
File Size = 500000 bytes
```

This value is encoded into the BMP image.

The decoder uses this information to determine exactly how many bytes must be extracted.

---

## 6. Check Image Capacity

Before modifying the image, the encoder verifies whether the BMP contains enough capacity for the complete payload.

The payload consists of:

```text
Magic String Size
+
Magic String
+
Extension Size
+
Extension
+
Secret File Size
+
Secret File Data
```

Each payload bit requires one image byte because one bit is stored in the image byte's LSB.

Therefore:

```text
1 secret byte → 8 image bytes
```

If the image does not have sufficient capacity, encoding is stopped.

---

## 7. Copy BMP Header

The BMP header is copied directly from the source image to the destination image.

For the supported BMP structure:

```text
54-byte BMP Header
```

is preserved.

The header is not modified during the encoding process.

After copying the header, the encoding process starts with the image data.

---

## 8. Encode Magic String Size

The size of the magic string is stored using 32 image bytes.

For example:

```text
Magic String:

MYSECRET

Size:

8
```

The 32-bit representation of the value is distributed across the LSBs of 32 image bytes.

Conceptually:

```text
32 image bytes
       |
       v
32 LSBs
       |
       v
Magic String Size
```

---

## 9. Encode Magic String

Each character requires 8 bits.

Therefore:

```text
1 character
     ↓
8 bits
     ↓
8 image bytes
```

For example:

```text
'M'
```

is converted into its binary representation, and each bit is stored in the LSB of a different image byte.

The process is repeated for every character in the magic string.

---

## 10. Encode Secret File Extension Size

The extension size is encoded before the extension itself.

For example:

```text
Extension:

.mp3

Size:

4
```

The extension size is stored using 32 image bytes.

This allows the decoder to determine how many characters must be extracted.

---

## 11. Encode Secret File Extension

The extension is encoded character-by-character.

For:

```text
.mp3
```

the encoder stores:

```text
.
m
p
3
```

Each character is encoded into 8 image bytes.

---

## 12. Encode Secret File Size

The complete secret file size is encoded into the image.

For example:

```text
Secret File Size = 500000 bytes
```

The decoder later uses this value to determine the exact amount of secret data to extract.

---

## 13. Encode Secret File Data

This is the main steganography operation.

The encoder reads the secret file one byte at a time.

Example:

```text
Secret byte
     |
     v
Convert to 8 bits
     |
     v
Store each bit in the LSB
of 8 image bytes
```

Conceptually:

```text
Secret Byte
01001011
   |
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
   +----> Image Byte LSB
```

The same process is repeated until the complete secret file has been encoded.

---

## 14. Copy Remaining Image Data

After the secret payload has been embedded, the remaining image bytes are copied directly from the source BMP to the stego BMP.

This produces the final:

```text
stego.bmp
```

The resulting file remains a valid BMP image.

---

# Decoding Process

The decoding operation extracts the hidden file from the stego image.

Command structure:

```text
./a.out -d stego.bmp output
```

The decoder performs the reverse operation of encoding.

---

## Decoding Workflow

```text
Start
  |
  v
Read Command-Line Arguments
  |
  v
Check Decode Operation (-d)
  |
  v
Validate Stego BMP
  |
  v
Open Stego Image
  |
  v
Read User Magic String
  |
  v
Skip BMP Header
  |
  v
Decode Magic String Size
  |
  v
Decode Magic String
  |
  v
Compare Magic Strings
  |
  +---- No ----> Stop Decoding
  |
 Yes
  |
  v
Decode Extension Size
  |
  v
Decode Extension
  |
  v
Decode Secret File Size
  |
  v
Create Output File
  |
  v
Decode Secret File Data
  |
  v
Close Files
  |
  v
Recovered File
  |
  v
End
```

---

## 1. Validate Input

Example:

```text
./a.out -d stego.bmp recovered
```

The decoder validates:

- Operation type
- Stego image
- Output file name

---

## 2. Open Stego Image

The stego image is opened in binary read mode:

```c
fopen(stego_image, "rb");
```

The decoder also accepts the magic string from the user.

---

## 3. Skip BMP Header

The first 54 bytes contain the BMP header.

Therefore, the decoder moves the file pointer to the beginning of the encoded image data:

```text
BMP Header
    |
    | 54 bytes
    v
Encoded Payload
```

The decoder then starts extracting the hidden information.

---

## 4. Decode Magic String Size

The decoder reads 32 image bytes and extracts their LSBs.

The extracted 32 bits are reconstructed into the original integer.

Example:

```text
32 image bytes
       |
       v
Extract LSBs
       |
       v
Magic String Size = 8
```

---

## 5. Decode Magic String

The decoder reads 8 image bytes for each character.

```text
8 image bytes
      |
      v
8 LSBs
      |
      v
1 character
```

The process continues until the complete magic string is reconstructed.

Example:

```text
Decoded Magic String:

MYSECRET
```

---

## 6. Verify Magic String

The decoder compares:

```text
User Entered Magic String
            |
            v
       strcmp()
            |
            v
Decoded Magic String
```

If both strings match:

```text
Magic string matched.
Continue decoding.
```

If they do not match:

```text
Magic string doesn't match.
Stop decoding.
```

This prevents the decoder from blindly extracting data from an unrelated BMP image.

---

## 7. Decode Secret File Extension

The decoder first extracts the extension size.

For example:

```text
Extension Size = 4
```

It then extracts four characters:

```text
.mp3
```

The extension is used to construct the recovered file name.

---

## 8. Decode Secret File Size

The decoder reconstructs the stored secret file size.

Example:

```text
Secret File Size = 500000 bytes
```

The decoder now knows exactly how many bytes must be recovered.

---

## 9. Create Output File

If the decoded extension is:

```text
.mp3
```

and the user supplied:

```text
recovered
```

the output file becomes:

```text
recovered.mp3
```

The output file is opened in binary write mode.

---

## 10. Decode Secret File Data

The decoder reads:

```text
8 image bytes
```

for every secret byte.

It extracts the LSB from each image byte:

```text
Image LSBs
01001011
    |
    v
Original Secret Byte
```

The reconstructed byte is written directly to the output file.

This process continues until the stored secret file size has been reached.

---

## 11. Recovered File

After all secret bytes have been decoded:

```text
stego.bmp
    |
    v
LSB Extraction
    |
    v
Recovered File
```

The recovered file retains the original file extension and binary content.

---

# Data Format

The encoded payload follows a predefined structure:

```text
+-----------------------------+
| Magic String Size           |
+-----------------------------+
| Magic String                |
+-----------------------------+
| Extension Size              |
+-----------------------------+
| Secret File Extension       |
+-----------------------------+
| Secret File Size            |
+-----------------------------+
| Secret File Data            |
+-----------------------------+
```

This structure allows the decoder to determine:

1. How large the magic string is.
2. What the magic string contains.
3. How large the file extension is.
4. What the secret file extension is.
5. How many bytes belong to the secret file.
6. Where the secret file data ends.

---

# LSB Encoding Concept

LSB stands for **Least Significant Bit**.

Consider an image byte:

```text
10110110
```

The last bit is the LSB:

```text
1011011[0]
```

If the secret bit is:

```text
1
```

the image byte becomes:

```text
10110111
```

Only one bit has changed.

The encoding process therefore modifies the least significant bit of image bytes to store secret information.

---

# Character Encoding

A character contains 8 bits.

For example:

```text
Character: A

ASCII:
01000001
```

The eight bits are stored across eight image bytes:

```text
Image Byte 1 → Bit 0
Image Byte 2 → Bit 1
Image Byte 3 → Bit 2
Image Byte 4 → Bit 3
Image Byte 5 → Bit 4
Image Byte 6 → Bit 5
Image Byte 7 → Bit 6
Image Byte 8 → Bit 7
```

The decoder performs the reverse operation.

---

# Integer Encoding

Integer values such as:

```text
Magic String Size
Extension Size
Secret File Size
```

are represented using 32 bits.

The encoder distributes these bits across 32 image bytes.

Conceptually:

```text
32-bit Integer
     |
     v
Extract individual bits
     |
     v
Store one bit per image byte
```

During decoding:

```text
Image Byte LSBs
      |
      v
32 reconstructed bits
      |
      v
Original Integer
```

---

# Binary File Processing

The project treats the secret file as raw binary data rather than as a C string.

Files can contain arbitrary byte values, so binary file operations are used:

```c
fopen()
fread()
fwrite()
fseek()
ftell()
fclose()
```

The secret file is opened using:

```text
"rb"
```

and the recovered file is written using:

```text
"wb"
```

This allows the same encoding algorithm to process:

```text
TXT
PDF
MP3
MP4
PNG
JPEG
```

without requiring separate encoding functions for each file type.

---

# Supported Secret File Types

The LSB encoding mechanism works with binary files, so the project can be used with different payload formats.

| File Type | Example |
|-----------|---------|
| Text | `secret.txt` |
| PDF | `document.pdf` |
| Audio | `song.mp3` |
| Video | `video.mp4` |
| PNG Image | `image.png` |
| JPEG Image | `photo.jpeg` |

The BMP image remains the carrier file.

```text
Carrier:
source.bmp

Secret:
document.pdf / song.mp3 / video.mp4 / image.png / photo.jpeg

Output:
stego.bmp
```

---

# File Capacity

The size of the secret file is limited by the available capacity of the carrier BMP image.

Since one secret byte contains 8 bits:

```text
1 secret byte
      ↓
8 secret bits
      ↓
8 image bytes
```

Therefore, larger secret files require larger carrier images.

The encoder checks the required capacity before starting the encoding process.

---

# Command-Line Usage

The application supports two primary operations:

```text
-e    Encode secret data into a BMP image

-d    Decode secret data from a stego BMP image
```

---

# 1. Encode

## Command

```bash
./a.out -e source.bmp secret_file stego.bmp
```

Example with a text file:

```bash
./a.out -e source.bmp secret.txt stego.bmp
```

Example with a PDF:

```bash
./a.out -e source.bmp document.pdf stego.bmp
```

Example with an MP3:

```bash
./a.out -e source.bmp song.mp3 stego.bmp
```

Example with an MP4:

```bash
./a.out -e source.bmp video.mp4 stego.bmp
```

The application asks for the magic string during encoding.

Example:

```text
Enter your magic string:
MYSECRET
```

After successful encoding:

```text
Encoding completed successfully.
```

---

# 2. Decode

## Command

```bash
./a.out -d stego.bmp recovered
```

The decoder asks for the magic string:

```text
Enter your magic string to decode:
MYSECRET
```

If the magic string matches the encoded value, decoding continues.

For example, if the hidden file was:

```text
song.mp3
```

the output will be:

```text
recovered.mp3
```

---

# Example Complete Workflow

```text
1. Select a BMP carrier image
       |
       v
2. Select a secret file
       |
       v
3. Run encoding operation
       |
       v
4. Enter magic string
       |
       v
5. Application checks image capacity
       |
       v
6. BMP header is preserved
       |
       v
7. Metadata is encoded
       |
       v
8. Secret file data is encoded
       |
       v
9. Stego BMP is generated
       |
       v
10. Run decoding operation
       |
       v
11. Enter the same magic string
       |
       v
12. Magic string is verified
       |
       v
13. File metadata is decoded
       |
       v
14. Secret file bytes are reconstructed
       |
       v
15. Recovered file is generated
```

---

# Example Session

## Encoding

```text
$ gcc main.c encode.c decode.c -o stego

$ ./stego -e source.bmp document.pdf stego.bmp

Enter your magic string:
MYSECRET

Encoding completed successfully.
```

## Decoding

```text
$ ./stego -d stego.bmp recovered

Enter your magic string to decode:
MYSECRET

Magic string matched.
Secret file extension = .pdf
Secret file size = 245760 bytes.

Decoding completed successfully.
```

The recovered file will be:

```text
recovered.pdf
```

---

# Testing

The project can be tested using different types of secret files.

### Text File Test

```bash
./stego -e source.bmp secret.txt stego.bmp
./stego -d stego.bmp recovered
```

Verify:

```text
secret.txt
     ↓
recovered.txt
```

### PDF Test

```bash
./stego -e source.bmp document.pdf stego.bmp
./stego -d stego.bmp recovered
```

Verify:

```text
document.pdf
     ↓
recovered.pdf
```

### MP3 Test

```bash
./stego -e source.bmp song.mp3 stego.bmp
./stego -d stego.bmp recovered
```

Verify:

```text
song.mp3
     ↓
recovered.mp3
```

### MP4 Test

```bash
./stego -e source.bmp video.mp4 stego.bmp
./stego -d stego.bmp recovered
```

Verify:

```text
video.mp4
     ↓
recovered.mp4
```

### PNG / JPEG Test

```bash
./stego -e source.bmp image.png stego.bmp
./stego -d stego.bmp recovered
```

or:

```bash
./stego -e source.bmp photo.jpeg stego.bmp
./stego -d stego.bmp recovered
```

The most reliable verification for binary files is a byte-for-byte comparison between the original and recovered files.

---

# Concepts Used

This project demonstrates practical implementation of the following C programming and systems-level concepts:

- C programming
- Command-line arguments
- File handling
- Binary file I/O
- `fopen()` and `fclose()`
- `fread()` and `fwrite()`
- `fseek()` and `ftell()`
- File size calculation
- Pointers
- Character arrays
- Strings
- String comparison
- Bitwise operators
- Bit manipulation
- Least Significant Bit processing
- Binary data processing
- BMP file handling
- Metadata encoding
- Metadata decoding
- Input validation
- Image capacity calculation
- Modular programming
- Header files
- Error handling
- Encoding and decoding algorithms

---

# Error Handling

The application performs validation and error handling for situations such as:

- Invalid command-line arguments
- Invalid operation type
- Invalid BMP input
- Unable to open source image
- Unable to open secret file
- Unable to create stego image
- Insufficient image capacity
- Unable to read image data
- Unable to read secret file data
- Invalid magic string
- Unable to create recovered file
- Unable to decode secret data

Example:

```text
Error: Insufficient capacity in the source image.
```

or:

```text
Magic string doesn't match.
```

---

# Implementation Highlights

The project follows a structured encoding and decoding protocol.

### Encoding

```text
BMP Header
    ↓
Magic String Size
    ↓
Magic String
    ↓
Extension Size
    ↓
Extension
    ↓
Secret File Size
    ↓
Secret File Data
    ↓
Remaining BMP Data
```

### Decoding

```text
Skip BMP Header
    ↓
Decode Magic String Size
    ↓
Decode Magic String
    ↓
Verify Magic String
    ↓
Decode Extension Size
    ↓
Decode Extension
    ↓
Decode Secret File Size
    ↓
Decode Secret File Data
    ↓
Create Recovered File
```

The decoder therefore follows the same sequence used by the encoder in reverse.

---

# Limitations

- The current implementation uses **BMP as the carrier image**.
- The BMP capacity limits the maximum size of the secret file.
- The implementation assumes the supported BMP structure used by the project.
- LSB steganography does not provide encryption; the hidden data is encoded rather than cryptographically encrypted.
- Very large secret files require sufficiently large carrier images.
- The project is primarily intended for learning and demonstrating C-based binary file processing, bit manipulation, and steganography concepts.

---

# Future Enhancements

Possible future improvements include:

- Password-based encryption before embedding.
- Support for additional carrier image formats.
- Improved BMP format and row-padding handling.
- Graphical user interface.
- Progress indicators for large files.
- Stronger payload validation.
- File integrity verification using checksums or hashes.
- Additional steganography techniques.
- Improved capacity calculation for different BMP formats.

---

# Author

**Dileep**
