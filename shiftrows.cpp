#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

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

void shiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

int main() {
    string input;
    unsigned char state[4][4];
    
    cout << "Enter 128-bit plaintext in hex format: ";
    getline(cin, input);
    
    try {
        hexStringToState(input, state);
    } catch (...) {
        cerr << "Error: Invalid hex input format\n";
        return 1;
    }
    
    printState(state, "Initial State");
    shiftRows(state);
    printState(state, "After ShiftRows");
    cout << "\nResult: " << stateToHexString(state) << "\n";
    
    ofstream outFile("shiftrows_output.txt");
    if (outFile.is_open()) {
        outFile << stateToHexString(state) << "\n";
        outFile.close();
        cout << "Output written to shiftrows_output.txt\n";
    }
    
    return 0;
}
