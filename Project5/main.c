/*
 * Project5.c
 *
 * Created: 3/10/2021 2:48:42 PM
 * Author : mylyn
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "avr.h"
#include "lcd.h"

enum color{YELLOW,RED,GREEN,BLUE};

int is_pressed(int r, int c){
	DDRC = 0; /* set C to be input */
	PORTC = 0; /* set all pins of PortC to be N/C" */
	
	SET_BIT(DDRC,r); /*set r bit to 0 */
	CLR_BIT(PORTC,r);
	
	CLR_BIT(DDRC,c+4); /* set c bit to w1*/
	SET_BIT(PORTC,c+4);
	
	avr_wait(25);
	
	if( GET_BIT(PINC,c+4)){
		return 0;
	}
	return 1;
}

int get_key(){
	
	int r;
	int c;
	for(r = 0; r < 4; r++){
		for(c = 0; c < 4; c++){
			
			if(is_pressed(r,c)){
				return 1 + c + r*4;
			}
		}
	}
	return 0;
}

void displayTitle(){
	lcd_clr();
	char buffer[32];
	sprintf(buffer,"Match the Lights");
	lcd_pos(0,0);
	lcd_puts(buffer);
	sprintf(buffer,"#:Start");
	lcd_pos(1,0);
	lcd_puts(buffer);
	
}

void displayRules(){
	lcd_clr();
	char buffer[32];
	sprintf(buffer,"Memorize &Repeat");
	lcd_pos(0,0);
	lcd_puts(buffer);
	sprintf(buffer,"No mistakes!");
	lcd_pos(1,0);
	lcd_puts(buffer);
}

void displayRound(int r){
	lcd_clr();
	char buffer[32];
	sprintf(buffer,"Round %d",r);
	lcd_pos(0,5);
	lcd_puts(buffer);
}

void displayStatus(int b){
	lcd_clr();
	char buffer[32];
	if(b == 0){
		sprintf(buffer, "You Lost :(");
		lcd_pos(0,5);
		lcd_puts(buffer);
	}
	else if(b == 1){
		sprintf(buffer,"You Won :)");
		lcd_pos(0,5);
		lcd_puts(buffer);
	}
	
	sprintf(buffer,"#:Replay; D:Quit");
	lcd_pos(1,0);
	lcd_puts(buffer);
}

void displayPlayer(int keypress){
	lcd_clr();
	char buffer[32];
	sprintf(buffer,"Player:%d D:Quit",keypress);
	lcd_pos(0,0);
	lcd_puts(buffer);
	sprintf(buffer,"1:B 2:G 3:R 4:Y");
	lcd_pos(1,0);
	lcd_puts(buffer);
}


int getRandom(){
	
	return rand() % 4 ;
}

int checkMatch(int* sequence,int* player,int n){
	int i;
	for(i = 0; i< n; i++){
		
		if(sequence[i] != (int)player[i]){
			avr_wait(1000);
			return 0;
		}
	}	
	
	return 1; //1 if match, 0 otherwise
}

int playerTurn(int* sequence,int n){
	int key;
	int numPress = 0;
	int player[n];
	while(numPress != n){
		
		key = get_key();
		displayPlayer(numPress);
		
		if(key == 1 ){
			player[numPress] = BLUE;
			avr_wait(500);
			numPress++;
		}
		else if(key == 2){
			player[numPress] = GREEN;
			avr_wait(500);
			numPress++;
		}
		else if(key == 3){
			player[numPress] = RED;
			avr_wait(500);
			numPress++;
		}
		else if(key == 4){
			player[numPress] = YELLOW;
			avr_wait(500);
			numPress++;
		}
		else if(key == 16){
			return 2;
		}
		
		
	}
	return checkMatch(sequence,player,n); // 0 if fail; 1 match; 2 quit
}

void blinkSequence(int* sequence,int n,int ms){
	int i;
	for(i = 0; i < n;i++){
		blink(ms,sequence[i]);
	}
}

int game(int ms, int totalRounds){
	//return 0 if lose; 1 otherwise
	int host[totalRounds] ;
	//int player[5];
	int round; 
	int p;
	for(round = 1; round < totalRounds + 1; round++){
		displayRound(round);
		int color = getRandom();
		host[round-1] = color;
		
		blinkSequence(host,round,ms);
		
		p = playerTurn(host,round);
		if(p == 2){
			return 2;
		}
		else if(p == 0){
			return 0;
		}
		avr_wait(500);
	}
	
	return 1;
}



int main(void)
{
    /* Replace with your application code */
	lcd_init();
	DDRA = 0b00011111;
	srand(time(0));
	int g;
    while (1) 
    {
		if(get_key() == 13){
			displayTitle();
		}
		else if(get_key() == 14){
			displayRules();
		}
		else if(get_key() == 16){
			lcd_clr();
		}
		else if(get_key() == 15){
			 g = game(500,5); //normal mode
			 if(g == 0){
				 displayStatus(0);
			 }
			 else if(g == 1){
				 displayStatus(1);
			 }
			 else{
				lcd_clr(); 
			 }
		}
    }
	
}

