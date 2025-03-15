#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void nums(int i){
    switch (i){
        case 0:
           cputs("1");
           break;
        case 1:
           cputs("2");
           break;
        case 2:
           cputs("3");
           break;
        case 3:
           cputs("4");
           break;
        case 4:
           cputs("5");
           break;
        case 5:
           cputs("6");
	   break;
        case 6:
           cputs("7");
	   break;
        case 7:
	   cputs("8");
           break;
        case 8:
           cputs("9");
           break;
        case 9:
           cputs("10");
           break;
        case 10:
           cputs("11");
           break;
        case 11:
           cputs("12");
           break;
        case 12:
           cputs("13");
           break;
        case 13:
           cputs("14");
           break;
        case 14:
           cputs("15");
           break;
        case 15:
           cputs("16");
           break;
        default:
           break;
    }  
}

void setColors(int i){
    switch (i){
        case 0:
           cputs("1\r");
           break;
        case 1:
           cputs("2\r");
           break;
        case 2:
           cputs("3\r");
           break;
        case 3:
           cputs("4\r");
           break;
        case 4:
           cputs("5\r");
           break;
        case 5:
            cputs("6\r");
	    break;
	case 6:
	   cputs("7\r");
	   break;
        case 7:
	   cputs("8\r");
           break;
        case 8:
           cputs("9\r");
           break;
        case 9:
           cputs("10\r");
           break;
        case 10:
           cputs("11\r");
           break;
        case 11:
           cputs("12\r");
           break;
        case 12:
           cputs("13\r");
           break;
        case 13:
           cputs("14\r");
           break;
        case 14:
           cputs("15\r");
           break;
        case 15:
           cputs("16\r");
           break;
        default:
           break;
    }  
}

int main(){
    textbackground(BLACK);
    textcolor(WHITE);
  
    clrscr();
    window(20, 5, 60, 15);
  
    int x = 1, y = 1;
    gotoxy(x, y);
  
    for(int i = 0; i <= 15; i++){
        textbackground(i);
        for(int j = 0; j <= 15; j++){
            textcolor(j);
            cputs("background: ");
            nums(i);
            cputs(" color: ");
            setColors(j);
            delay(500);
            gotoxy(x,y--);
            if(y < 1){
	      insline();
	      insline();
	      insline();
	    }
	}
        y += 3;
    }
    getch();
    return 0; 
}