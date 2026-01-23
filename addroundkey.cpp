#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

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

void addRoundKey(unsigned char state[4][4], unsigned char roundKey[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

int main() {
    string stateInput;
    string keyInput;
    unsigned char state[4][4];
    unsigned char roundKey[4][4];
    
    cout << "Enter 128-bit state in hex format: ";
    getline(cin, stateInput);
    
    cout << "Enter 128-bit round key in hex format: ";
    getline(cin, keyInput);
    
    try {
        hexStringToState(stateInput, state);
        hexStringToState(keyInput, roundKey);
    } catch (...) {
        cerr << "Error: Invalid hex input format\n";
        return 1;
    }
    
    printState(state, "Initial State");
    printState(roundKey, "Round Key");
    
    addRoundKey(state, roundKey);
    
    printState(state, "After AddRoundKey");
    cout << "\nResult: " << stateToHexString(state) << "\n";
    
    return 0;
}
