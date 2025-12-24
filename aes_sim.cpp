#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>

using namespace std;

// AES S-Box
const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// AES Inverse S-Box
const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Rcon for key expansion
const uint8_t Rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// Utility functions
void hexStringToState(const string& hex, uint8_t state[4][4]) {
    string cleanHex = hex;
    
    // Remove "0x" or "0X" prefix if present
    if (cleanHex.substr(0, 2) == "0x" || cleanHex.substr(0, 2) == "0X") {
        cleanHex = cleanHex.substr(2);
    }
    
    // Remove all spaces from the hex string
    cleanHex.erase(remove(cleanHex.begin(), cleanHex.end(), ' '), cleanHex.end());
    
    // Convert hex string to bytes sequentially, then arrange in column-major order
    uint8_t bytes[16];
    for (int i = 0; i < 16; i++) {
        string byteString = cleanHex.substr(i * 2, 2);
        bytes[i] = (uint8_t)strtoul(byteString.c_str(), nullptr, 16);
    }
    
    // Now fill state matrix column by column
    // Bytes 0-3 go to column 0, bytes 4-7 to column 1, etc.
    int byteIndex = 0;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            state[row][col] = bytes[byteIndex++];
        }
    }
}

string stateToHexString(uint8_t state[4][4]) {
    stringstream ss;
    ss << "0x";
    // Read column by column to maintain byte order
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            ss << hex << setw(2) << setfill('0') << (int)state[row][col];
        }
    }
    return ss.str();
}

void printState(uint8_t state[4][4], const string& label) {
    cout << "\n" << label << ":\n";
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            cout << hex << setw(2) << setfill('0') << (int)state[row][col] << " ";
        }
        cout << endl;
    }
    cout << "Hex format: " << stateToHexString(state) << endl;
}

// ============ AES OPERATIONS ============

// Task 9: SubBytes
void subBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// Task 10: ShiftRows
void shiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    
    // Row 1: shift left by 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    
    // Row 2: shift left by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    // Row 3: shift left by 3 (or right by 1)
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

// Galois Field multiplication
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) {
            a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return p;
}

// Task 11: MixColumns
void mixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    
    for (int col = 0; col < 4; col++) {
        temp[0] = gmul(state[0][col], 2) ^ gmul(state[1][col], 3) ^ 
                  state[2][col] ^ state[3][col];
        temp[1] = state[0][col] ^ gmul(state[1][col], 2) ^ 
                  gmul(state[2][col], 3) ^ state[3][col];
        temp[2] = state[0][col] ^ state[1][col] ^ 
                  gmul(state[2][col], 2) ^ gmul(state[3][col], 3);
        temp[3] = gmul(state[0][col], 3) ^ state[1][col] ^ 
                  state[2][col] ^ gmul(state[3][col], 2);
        
        for (int row = 0; row < 4; row++) {
            state[row][col] = temp[row];
        }
    }
}

// Task 12: AddRoundKey
void addRoundKey(uint8_t state[4][4], uint8_t roundKey[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

// ============ INVERSE OPERATIONS ============

// Task 13: Inverse SubBytes
void invSubBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

// Task 14: Inverse ShiftRows
void invShiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    
    // Row 1: shift right by 1
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    
    // Row 2: shift right by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    // Row 3: shift right by 3 (or left by 1)
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

// Task 15: Inverse MixColumns
void invMixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    
    for (int col = 0; col < 4; col++) {
        temp[0] = gmul(state[0][col], 0x0e) ^ gmul(state[1][col], 0x0b) ^ 
                  gmul(state[2][col], 0x0d) ^ gmul(state[3][col], 0x09);
        temp[1] = gmul(state[0][col], 0x09) ^ gmul(state[1][col], 0x0e) ^ 
                  gmul(state[2][col], 0x0b) ^ gmul(state[3][col], 0x0d);
        temp[2] = gmul(state[0][col], 0x0d) ^ gmul(state[1][col], 0x09) ^ 
                  gmul(state[2][col], 0x0e) ^ gmul(state[3][col], 0x0b);
        temp[3] = gmul(state[0][col], 0x0b) ^ gmul(state[1][col], 0x0d) ^ 
                  gmul(state[2][col], 0x09) ^ gmul(state[3][col], 0x0e);
        
        for (int row = 0; row < 4; row++) {
            state[row][col] = temp[row];
        }
    }
}

// Task 16: Key Expansion
void keyExpansion(const uint8_t key[16], uint8_t roundKeys[11][4][4]) {
    uint8_t temp[4];
    
    // First round key is the original key - fill column by column
    int keyIndex = 0;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            roundKeys[0][row][col] = key[keyIndex++];
        }
    }
    
    for (int round = 1; round <= 10; round++) {
        // Copy last column of previous round key
        for (int i = 0; i < 4; i++) {
            temp[i] = roundKeys[round - 1][i][3];
        }
        
        // RotWord
        uint8_t t = temp[0];
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = temp[3];
        temp[3] = t;
        
        // SubWord
        for (int i = 0; i < 4; i++) {
            temp[i] = sbox[temp[i]];
        }
        
        // XOR with Rcon
        temp[0] ^= Rcon[round];
        
        // Generate new round key
        for (int col = 0; col < 4; col++) {
            if (col == 0) {
                for (int row = 0; row < 4; row++) {
                    roundKeys[round][row][col] = roundKeys[round - 1][row][col] ^ temp[row];
                }
            } else {
                for (int row = 0; row < 4; row++) {
                    roundKeys[round][row][col] = roundKeys[round - 1][row][col] ^ 
                                                  roundKeys[round][row][col - 1];
                }
            }
        }
    }
}

// ============ MAIN PROGRAM ============

int main() {
    int choice;
    string input;
    uint8_t state[4][4];
    
    cout << "========================================\n";
    cout << "   AES Cryptography Implementation\n";
    cout << "========================================\n\n";
    
    cout << "Select operation:\n";
    cout << "1. Full AES Encryption (10 rounds)\n";
    cout << "2. Full AES Decryption (10 rounds)\n";
    cout << "\nEnter choice: ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        cout << "\nEnter 128-bit plaintext in hex format: ";
        getline(cin, input);
        hexStringToState(input, state);
        
        string keyInput;
        cout << "Enter 128-bit AES Key in hex format: ";
        getline(cin, keyInput);
        
        int numRounds;
        cout << "Enter number of rounds (1-10): ";
        cin >> numRounds;
        if (numRounds < 1 || numRounds > 10) {
            cout << "Invalid number of rounds. Using 10 rounds.\n";
            numRounds = 10;
        }
        
        uint8_t key[16];
        string cleanHex = keyInput;
        if (cleanHex.substr(0, 2) == "0x" || cleanHex.substr(0, 2) == "0X") {
            cleanHex = cleanHex.substr(2);
        }
        // Remove spaces
        cleanHex.erase(remove(cleanHex.begin(), cleanHex.end(), ' '), cleanHex.end());
        
        for (int i = 0; i < 16; i++) {
            string byteString = cleanHex.substr(i * 2, 2);
            key[i] = (uint8_t)strtoul(byteString.c_str(), nullptr, 16);
        }
        
        uint8_t roundKeys[11][4][4];
        keyExpansion(key, roundKeys);
        
        printState(state, "Initial State");
        
        // Initial round
        addRoundKey(state, roundKeys[0]);
        printState(state, "After Initial AddRoundKey (Round 0)");
        
        // Main rounds
        for (int round = 1; round < numRounds; round++) {
            cout << "\n========== ROUND " << dec << round << " ==========\n";
            
            subBytes(state);
            printState(state, "After SubBytes");
            
            shiftRows(state);
            printState(state, "After ShiftRows");
            
            mixColumns(state);
            printState(state, "After MixColumns");
            
            addRoundKey(state, roundKeys[round]);
            printState(state, "After AddRoundKey");
        }
        
        // Final round (no MixColumns)
        cout << "\n========== ROUND " << dec << numRounds << " (Final) ==========\n";
        subBytes(state);
        printState(state, "After SubBytes");
        
        shiftRows(state);
        printState(state, "After ShiftRows");
        
        addRoundKey(state, roundKeys[numRounds]);
        printState(state, "After Final AddRoundKey");
        
        cout << "\n========================================\n";
        cout << "CIPHERTEXT (128-bit): " << stateToHexString(state) << endl;
        cout << "========================================\n";
    }
    else if (choice == 2) {
        cout << "\nEnter 128-bit ciphertext in hex format: ";
        getline(cin, input);
        hexStringToState(input, state);
        
        string keyInput;
        cout << "Enter 128-bit AES Key in hex format: ";
        getline(cin, keyInput);
        
        int numRounds;
        cout << "Enter number of rounds (1-10): ";
        cin >> numRounds;
        if (numRounds < 1 || numRounds > 10) {
            cout << "Invalid number of rounds. Using 10 rounds.\n";
            numRounds = 10;
        }
        
        uint8_t key[16];
        string cleanHex = keyInput;
        if (cleanHex.substr(0, 2) == "0x" || cleanHex.substr(0, 2) == "0X") {
            cleanHex = cleanHex.substr(2);
        }
        // Remove spaces
        cleanHex.erase(remove(cleanHex.begin(), cleanHex.end(), ' '), cleanHex.end());
        
        for (int i = 0; i < 16; i++) {
            string byteString = cleanHex.substr(i * 2, 2);
            key[i] = (uint8_t)strtoul(byteString.c_str(), nullptr, 16);
        }
        
        uint8_t roundKeys[11][4][4];
        keyExpansion(key, roundKeys);
        
        printState(state, "Initial Ciphertext");
        
        // Initial round
        addRoundKey(state, roundKeys[numRounds]);
        printState(state, "After Initial AddRoundKey (Round " + to_string(numRounds) + ")");
        
        // Main rounds (in reverse)
        for (int round = numRounds - 1; round >= 1; round--) {
            cout << "\n========== ROUND " << dec << (numRounds - round) << " ==========\n";
            
            invShiftRows(state);
            printState(state, "After Inverse ShiftRows");
            
            invSubBytes(state);
            printState(state, "After Inverse SubBytes");
            
            addRoundKey(state, roundKeys[round]);
            printState(state, "After AddRoundKey");
            
            invMixColumns(state);
            printState(state, "After Inverse MixColumns");
        }
        
        // Final round (no InvMixColumns)
        cout << "\n========== ROUND " << dec << numRounds << " (Final) ==========\n";
        invShiftRows(state);
        printState(state, "After Inverse ShiftRows");
        
        invSubBytes(state);
        printState(state, "After Inverse SubBytes");
        
        addRoundKey(state, roundKeys[0]);
        printState(state, "After Final AddRoundKey");
        
        cout << "\n========================================\n";
        cout << "OUTPUT (128-bit): " << stateToHexString(state) << endl;
        cout << "========================================\n";
    }
    
    return 0;
}