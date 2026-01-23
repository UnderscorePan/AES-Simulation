#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

inline unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

inline void hexStringToState(const string& hex, unsigned char state[4][4]) {
    string cleanHex = hex;
    
    if (cleanHex.length() >= 2 && (cleanHex.substr(0, 2) == "0x" || cleanHex.substr(0, 2) == "0X")) {
        cleanHex = cleanHex.substr(2);
    }
    
    cleanHex.erase(remove(cleanHex.begin(), cleanHex.end(), ' '), cleanHex.end());
    
    unsigned char bytes[16];
    for (int i = 0; i < 16; i++) {
        string byteString = cleanHex.substr(i * 2, 2);
        bytes[i] = (unsigned char)strtoul(byteString.c_str(), nullptr, 16);
    }
    
    int byteIndex = 0;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            state[row][col] = bytes[byteIndex++];
        }
    }
}

inline string stateToHexString(unsigned char state[4][4]) {
    stringstream ss;
    ss << "0x";
    ss << uppercase;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            ss << hex << setw(2) << setfill('0') << (int)state[row][col];
        }
    }
    return ss.str();
}

inline void printState(unsigned char state[4][4], const string& label) {
    cout << "\n" << label << ":\n";
    for (int row = 0; row < 4; row++) {
        cout << hex << uppercase;
        for (int col = 0; col < 4; col++) {
            cout << setw(2) << setfill('0') << (int)state[row][col] << " ";
        }
        cout << "\n";
    }
    cout << "Hex format (128-bit): " << stateToHexString(state) << "\n";
}

void invMixColumns(unsigned char state[4][4]) {
    unsigned char temp[4];
    
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

int main() {
    string input;
    unsigned char state[4][4];
    
    cout << "Enter 128-bit ciphertext in hex format: ";
    getline(cin, input);
    
    try {
        hexStringToState(input, state);
    } catch (...) {
        cerr << "Error: Invalid hex input format\n";
        return 1;
    }
    
    printState(state, "Initial State");
    invMixColumns(state);
    printState(state, "After Inverse MixColumns");
    cout << "\nResult: " << stateToHexString(state) << "\n";
    
    return 0;
}
