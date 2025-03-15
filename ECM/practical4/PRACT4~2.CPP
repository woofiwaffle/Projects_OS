#include "dos.h"
#include "conio.h"
#include "stdio.h"

int code();

int keypressed();

int main(){
    textbackground(BLACK);
	
    window(20, 5, 60, 15);
    clrscr();
    
    int x = 1, y = 1, ch = 0;
    int left = 0;
    do{
       do{
          clrscr();
	  if((left == 1) && ((x - 1) >= 0)){
	    x -= 1;
	  }
	  if((left == 0) && ((x + 1) <= (60 - 20))){
	    x += 1;
	  }
	  gotoxy(x, y);
	  putch('*');
	  delay(100);
       } while(keypressed() == 0);
       ch = code();

       switch(ch){
	  case 59:{
	    left = 1;
	    break;
	  }
	  case 60:{
	    left = 0;
	    break;
	  }
       }
    } while (ch != 27);
    return 0;
}

int keypressed(){
	union REGS r;
	r.h.ah = 0x0B;
	int86(0x21, &r, &r);
	return r.h.al;
}

int code(){
	union REGS r;
	r.h.ah = 0x7;
	int86(0x21, &r, &r);
	return r.h.al;
}