// aes_decrypt.cpp
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
using namespace std;

// S-box table 
static const uint8_t sbox[256] = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,
  0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,
  0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,
  0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,
  0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,
  0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,
  0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,
  0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,
  0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,
  0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,
  0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,
  0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,
  0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,
  0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,
  0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,
  0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,
  0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// Inverse S-box table
static const uint8_t inv_sbox[256] = {
  0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,
  0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
  0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,
  0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
  0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,
  0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
  0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,
  0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
  0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,
  0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
  0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,
  0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
  0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,
  0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
  0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,
  0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
  0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,
  0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
  0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,
  0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
  0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,
  0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
  0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,
  0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
  0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,
  0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
  0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,
  0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
  0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,
  0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
  0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,
  0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

static const uint8_t Rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

class AES {
public:
    // keySize in bytes: 16, 24, or 32
    AES(const uint8_t* key, int keySize) {
        Nk = keySize / 4;
        if (Nk == 4) Nr = 10;
        else if (Nk == 6) Nr = 12;
        else if (Nk == 8) Nr = 14;
        else {
            cout << "Invalid key size!" << endl;
            exit(1);
        }
        KeyExpansion(key);
    }

    // Decrypt a single 16-byte block (in-place)
    void decryptBlock(uint8_t* block) {
        uint8_t state[4][4];
        for (int i = 0; i < 16; i++)
            state[i % 4][i / 4] = block[i];

        AddRoundKey(state, Nr);
        for (int round = Nr - 1; round >= 1; round--) {
            InvShiftRows(state);
            InvSubBytes(state);
            AddRoundKey(state, round);
            InvMixColumns(state);
        }
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, 0);

        for (int i = 0; i < 16; i++)
            block[i] = state[i % 4][i / 4];
    }

    // Utility: print block as hex bytes.
    void printBlock(const uint8_t* block) {
        for (int i = 0; i < 16; i++)
            cout << hex << setw(2) << setfill('0') << (int)block[i] << " ";
        cout << dec << endl;
    }

private:
    int Nk, Nr;
    uint8_t RoundKey[240];

    void KeyExpansion(const uint8_t* key) {
        int i = 0;
        while (i < Nk) {
            RoundKey[4*i + 0] = key[4*i + 0];
            RoundKey[4*i + 1] = key[4*i + 1];
            RoundKey[4*i + 2] = key[4*i + 2];
            RoundKey[4*i + 3] = key[4*i + 3];
            i++;
        }
        i = Nk;
        int totalWords = 4 * (Nr + 1);
        uint8_t temp[4];
        while (i < totalWords) {
            temp[0] = RoundKey[4*(i-1) + 0];
            temp[1] = RoundKey[4*(i-1) + 1];
            temp[2] = RoundKey[4*(i-1) + 2];
            temp[3] = RoundKey[4*(i-1) + 3];

            if (i % Nk == 0) {
                uint8_t t = temp[0];
                temp[0] = temp[1];
                temp[1] = temp[2];
                temp[2] = temp[3];
                temp[3] = t;
                temp[0] = sbox[temp[0]];
                temp[1] = sbox[temp[1]];
                temp[2] = sbox[temp[2]];
                temp[3] = sbox[temp[3]];
                temp[0] ^= Rcon[i/Nk];
            } else if (Nk > 6 && (i % Nk) == 4) {
                temp[0] = sbox[temp[0]];
                temp[1] = sbox[temp[1]];
                temp[2] = sbox[temp[2]];
                temp[3] = sbox[temp[3]];
            }
            RoundKey[4*i + 0] = RoundKey[4*(i - Nk) + 0] ^ temp[0];
            RoundKey[4*i + 1] = RoundKey[4*(i - Nk) + 1] ^ temp[1];
            RoundKey[4*i + 2] = RoundKey[4*(i - Nk) + 2] ^ temp[2];
            RoundKey[4*i + 3] = RoundKey[4*(i - Nk) + 3] ^ temp[3];
            i++;
        }
    }

    void InvSubBytes(uint8_t state[4][4]) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                state[i][j] = inv_sbox[state[i][j]];
    }

    void InvShiftRows(uint8_t state[4][4]) {
        uint8_t temp;
        // Row 1: shift right by 1
        temp = state[1][3];
        state[1][3] = state[1][2];
        state[1][2] = state[1][1];
        state[1][1] = state[1][0];
        state[1][0] = temp;
        // Row 2: shift right by 2
        uint8_t t0 = state[2][0], t1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = t0;
        state[2][3] = t1;
        // Row 3: shift right by 3 (left shift by 1)
        temp = state[3][0];
        state[3][0] = state[3][1];
        state[3][1] = state[3][2];
        state[3][2] = state[3][3];
        state[3][3] = temp;
    }

    // Multiply two numbers in GF(2^8)
    uint8_t multiply(uint8_t a, uint8_t b) {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            if (b & 1)
                result ^= a;
            bool hi_bit = (a & 0x80) != 0;
            a <<= 1;
            if (hi_bit)
                a ^= 0x1b;
            b >>= 1;
        }
        return result;
    }

    void InvMixColumns(uint8_t state[4][4]) {
        for (int c = 0; c < 4; c++) {
            uint8_t a0 = state[0][c], a1 = state[1][c],
                    a2 = state[2][c], a3 = state[3][c];
            state[0][c] = multiply(a0,0x0e) ^ multiply(a1,0x0b) ^ multiply(a2,0x0d) ^ multiply(a3,0x09);
            state[1][c] = multiply(a0,0x09) ^ multiply(a1,0x0e) ^ multiply(a2,0x0b) ^ multiply(a3,0x0d);
            state[2][c] = multiply(a0,0x0d) ^ multiply(a1,0x09) ^ multiply(a2,0x0e) ^ multiply(a3,0x0b);
            state[3][c] = multiply(a0,0x0b) ^ multiply(a1,0x0d) ^ multiply(a2,0x09) ^ multiply(a3,0x0e);
        }
    }

    void AddRoundKey(uint8_t state[4][4], int round) {
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                state[r][c] ^= RoundKey[round * 16 + c * 4 + r];
    }
};

////////////////////
// Main function:
int main() {
    int keySize;
    cout << "Enter key size (16, 24, or 32 bytes): ";
    cin >> keySize;
    if(keySize != 16 && keySize != 24 && keySize != 32) {
        cout << "Invalid key size!" << endl;
        return 1;
    }
    
    uint8_t key[32];
    cout << "Enter key in hex (without spaces, " << keySize*2 << " hex digits): ";
    string keyHex;
    cin >> keyHex;
    for (int i = 0; i < keySize; i++) {
        string byteStr = keyHex.substr(i*2, 2);
        key[i] = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
    }
    
    uint8_t ciphertext[16];
    cout << "Enter ciphertext block in hex (32 hex digits): ";
    string cipherHex;
    cin >> cipherHex;
    for (int i = 0; i < 16; i++) {
        string byteStr = cipherHex.substr(i*2, 2);
        ciphertext[i] = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
    }
    
    AES aes(key, keySize);
    cout << "Ciphertext:       ";
    aes.printBlock(ciphertext);
    
    aes.decryptBlock(ciphertext);
    
    cout << "Decrypted Plaintext: ";
    aes.printBlock(ciphertext);
    
    return 0;
}

/*OUTPUT*/
/*
Enter key size (16, 24, or 32 bytes): 24
Enter key in hex (without spaces, 48 hex digits): 00112233445566778899AABBCCDDEEFF0011223344556677
Enter ciphertext block in hex (32 hex digits): 491a6b83abf6a17b99c7571de0aafd43
Ciphertext:       49 1a 6b 83 ab f6 a1 7b 99 c7 57 1d e0 aa fd 43 
Decrypted Plaintext: 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00 
*/