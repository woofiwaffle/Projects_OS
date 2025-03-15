#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

int min_sec = 5;
int max_sec = 10;
int ticker;

int min_freq = 400;
int max_freq = 800;
int freq;

int dot_delay = 100;
int line_delay = 200;
int signal_delay = 200;
int char_delay = 300;

void interrupt (*old8)(...);
void interrupt new8(...);

void randomSec() {
    int sec = rand() % (max_sec - min_sec + 1) + min_sec;
    ticker = sec + 18;
}

void randomFreq() {
    freq = rand() % (max_freq + 1 - min_freq) + min_freq;
}

void boom() {
    sound(freq - 200);
    delay(dot_delay);
    nosound();
}

void tik() {
    sound(freq + 400);
    delay(line_delay);
    nosound();
}

void tik_tak() {
    tik();
    delay(signal_delay);
    tik();
    delay(signal_delay);
    boom();
    delay(char_delay);
}

void interrupt new8(...) {
    --ticker;
    if (ticker <= 0) {
        setvect(0x8, old8);
        nosound();
    }
    old8();
}

void main() {
    srand((unsigned)time(NULL));

    randomSec();

    clrscr();
    cprintf("EXIT - click ESC\n");
    old8 = getvect(0x8);
    setvect(0x8, new8);

    while (1) {
        if (ticker <= 0) {
            randomFreq();
            tik_tak();
            randomSec();
        }
        if (kbhit() && getch() == 27) {
            break;
        }
    }
    setvect(0x8, old8);
    exit(0);
}