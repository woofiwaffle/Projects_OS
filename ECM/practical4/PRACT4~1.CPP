#include "dos.h"
#include "conio.h"
#include "stdio.h"

int main(){
    textbackground(BLACK);
	
    window(20, 5, 60, 15);
    clrscr();
	
    int x = 1, y = 1, ch = 0;
    int left = 0;
    do{
       do{
         clrscr();
	 if((left==1)&&((x-1)>=0)){
	    x-=1;
	 }
	 if((left==0)&&((x+1)<=(60-20))){
	    x+=1;
         }
         gotoxy(x, y);
	 putch('*');
	 delay(100);
       }while(kbhit()==0);
       ch = getch();

       if(ch == 0 || ch == 224){
	  switch (getch ()){
                 case 59:{
		   left = 1;
		   break;
		 }

		 case 60:{
		   left = 0;
		   break;
		 }
	  }
	}
    }while(ch!=27);
    return 0;
}