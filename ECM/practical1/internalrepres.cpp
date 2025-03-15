#include "internalrepres.h"
#include <iostream>



string internalrepres::FloatToBinary(float floatNumber) {

    unsigned int floatBits;
    memcpy(&floatBits, &floatNumber, sizeof(floatNumber));
    /* memcpy - for bitwise representation of numbers
     * &floatBits - pointer to the memory area into which the binary representation floatNumber is copied
     * &floatNumber - pointer to the memory area from which the binary representation of floatNumber was copied
     * sizeof(floatNumber) - size in bytes */

    int sign = (floatBits >> 31) & 1;
    int exponent = (floatBits >> 23) & 0xFF; // 0xFF - 255 (IEEE754)
    int mantissa = floatBits & 0x7FFFFF;

    string signString = to_string(sign);
    string exponentString;
    for(int i = 7; i >= 0; --i){
        exponentString += ((exponent >> i) & 1) ? '1' : '0';
    }
    string mantissaString;
    for(int i = 22; i >= 0; --i){
        mantissaString += ((mantissa >> i) & 1) ? '1' : '0';
    }

    string binaryString = signString + " " + exponentString + " " + mantissaString;

    return binaryString;
}



string internalrepres::UnsignedCharToBinary(unsigned char ucharNumber) {

    string binaryString;

    for(int i = 7; i >= 0; --i){
        binaryString += ((ucharNumber >> i) & 1) ? '1' : '0';
    }

   return binaryString;
}



void internalrepres::SetBitsU(unsigned char& ucharNumber) {

    int startBit, numBits;

    cout << "Enter the most significant bit (0-7): ";
    cin >> startBit;

    cout << "Enter the number of bits to set(max.8): ";
    cin >> numBits;

    if(startBit < 0|| startBit >= 8 || numBits <= 0 || numBits > 8 || startBit - numBits + 1 < 0){
        cout << "Invalid" << endl;
        return;
    }

    unsigned char mask = 1 << startBit;

    for(int i = 0; i < numBits; ++i){
        cout << "Enter value for bit (0 or 1): " << (startBit - i) << ": ";
        int newBit;
        cin >> newBit;
        if(newBit != 0 && newBit != 1){
            cout << "Enter a valid value (0 or 1) for the bit" << endl;
            return;
        }
        if(newBit == 1){
            ucharNumber |= mask;
        }
        else{
            ucharNumber &= ~mask; //inversion, changes from 0 to 1 and from 1 to 0
        }
        mask >>= 1;
    }
}



void internalrepres::SetBitsF(float& floatNumber) {

    unsigned int floatBits;
    memcpy(&floatBits, &floatNumber, sizeof(floatNumber));

    int startBit, numBits;

    cout << "Enter the most significant bit (0-31): ";
    cin >> startBit;

    cout << "Enter the number of bits to set(max.32): ";
    cin >> numBits;

    if(startBit < 0 || startBit >= 32 || numBits <= 0 || numBits > 32 || startBit - numBits + 1 < 0){
        cout << "Invalid" << endl;
        return;
    }

    unsigned int mask = 1 << startBit;

    for(int i = 0; i < numBits; ++i){
        cout << "Enter value for bit (0 or 1): " << (startBit - i) << ": ";
        int newBit;
        cin >> newBit;
        if(newBit != 0 && newBit != 1){
            cout << "Enter a valid value (0 or 1) for the bit" << endl;
            return;
        }
        if(newBit == 1){
            floatBits |= mask;
        }
        else{
            floatBits &= ~mask;
        }
        mask >>= 1;
    }

    memcpy(&floatNumber, &floatBits, sizeof(floatNumber));
}