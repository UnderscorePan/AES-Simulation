# AES Cryptography Implementation

C++ implementation of AES encryption/decryption for university cryptography assignment.

## Features
- Full AES-128 encryption (configurable rounds 1-10)
- Full AES-128 decryption (configurable rounds 1-10)
- Detailed step-by-step output for each round
- Standard hex input/output for interoperability

## Compilation
```bash
g++ -o aes_sim aes_sim.cpp
```

## Usage
```bash
./aes_sim
```

## Input Format
- Plaintext/Ciphertext: `0x000102030405060708090A0B0C0D0E0F` or with spaces
- Key: `01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01`

## Assignment Details
Course: CryptoGraphy 2025/2026 Term 2530
Tasks: AES Encryption/Decryption