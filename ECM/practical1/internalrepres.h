#ifndef PRACTICAL1_INTERNALREPRES_H
#define PRACTICAL1_INTERNALREPRES_H

#include <string>

using namespace std;

class internalrepres {
public:
    static string FloatToBinary(float floatNumber);
    static string UnsignedCharToBinary(unsigned char ucharNumber);
    static void SetBitsU(unsigned  char& ucharNumber);
    static void SetBitsF(float& floatNumber);
};

#endif //PRACTICAL1_INTERNALREPRES_H
