#include "application.h"
#include "internalrepres.h"

#include <iostream>
#include <limits>

using namespace std;


int application::exec() {

    unsigned char ucharNumber = 0;
    float floatNumber = 0.0f;

    char ch;
    bool exit = false;
    while(!exit){
        ch = menu();
        switch(ch){
            case '1':{
                cout << "Enter number (float): ";
                if(!(cin >> floatNumber)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cerr << "Invalid" << endl;
                    break;
                }

                string binaryRepresentation = internalrepres::FloatToBinary(floatNumber);
                cout << "Binary: " << "" << binaryRepresentation << endl;
                break;
            }
            case '2':{
                unsigned int inputUcharNumber;
                cout << "Enter number (unsigned char): ";
                if(!(cin >> inputUcharNumber)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cerr << "Invalid" << endl;
                    break;
                }
                else{
                    ucharNumber = static_cast<unsigned char>(inputUcharNumber);
                    string binaryRepresentation = internalrepres::UnsignedCharToBinary(ucharNumber);
                    cout << "Binary: "<< "" << binaryRepresentation << endl;
                }
                break;
            }
            case '3':{
                int dataTypeChoice;
                cout << "Enter data type (1 - unsigned char, 2 - float): ";
                cin >> dataTypeChoice;
                if(dataTypeChoice == 1){
                    internalrepres::SetBitsU(ucharNumber);
                    string binaryRepresentation = internalrepres::UnsignedCharToBinary(ucharNumber);
                    cout << "Binary: "<< "" << binaryRepresentation << endl;
                    cout << "Number: " << "" << static_cast<int>(ucharNumber) << endl;
                }
                else if(dataTypeChoice == 2){
                    internalrepres::SetBitsF(floatNumber);
                    string binaryRep = internalrepres::FloatToBinary(floatNumber);
                    cout << "Binary: " << "" << binaryRep << endl;
                    cout << "Number: " << "" << static_cast<float>(floatNumber) << endl;
                }
                else{
                    cout << "Incorrect data type" << endl;
                }
                break;
            }
            case '0':{
                std::cout << "Exit" << std::endl;
                exit = true;
                break;
            }
            default:{
                cout << "Error\n";
                break;
            }
        }
    }
}



int application::menu(){
    char ch;
    cout << "--------------------------------------------" << endl;
    cout << "|Binary representation:" << endl;
    cout << "|1 - float" << endl;
    cout << "|2 - unsigned char" << endl;
    cout << "|3 - set a bit to a specific position" << endl;
    cout << "|0 - Exit" << endl << "> ";
    cin >> ch;
    cout << "--------------------------------------------" << endl;
    return ch;
}