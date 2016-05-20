#include <stdlib.h>
#include <stdio.h>
#include <3ds.h>
#include <math.h>
#include "catch.h"
#include "pid.h"

#define DEX 721

#define DELAY 25

void check(int number[]) {
	if ((number[0] * 100 + number[1] * 10 + number[2]) > DEX) {
		number[0] = 7;
		number[1] = 2;
		number[2] = 1;
	}
	if ((number[0] * 100 + number[1] * 10 + number[2]) == 0) {
		number[0] = 0;
		number[1] = 0;
		number[2] = 1;
	}
}

void printCursoreC(char cur[]) {
	printf("\x1b[3;24H%c%c%c", cur[0], cur[1], cur[2]);
	printf("\x1b[7;33H%c", cur[3]);
	printf("\x1b[9;15H%c", cur[4]);
	printf("\x1b[11;18H%c", cur[5]);
	printf("\x1b[13;17H%c", cur[6]);
}

void showC(int number[], int ratio[], int HP_perc, float bonusballvett[], int bonusindex, float status[], int statusindex, int r) {
	printf("\x1b[2;0HNumber in National Dex: \x1b[32m%d%d%d\x1b[0m\n\nHas a catch rate of \x1b[32m%d\x1b[0m  ", number[0], number[1], number[2], ratio[(number[0] * 100 + number[1] * 10 + number[2]) - 1]);
	printf("\n\nApprossimative HP percentage is: \x1b[32m%d\x1b[0m%%  ", HP_perc);
	printf("\n\nBall bonus is: \x1b[32m%4.2f\x1b[0m   ", bonusballvett[bonusindex]);
	printf("\n\nStatus' bonus is: \x1b[32m%4.2f\x1b[0m  ", status[statusindex]);
	printf("\n\nNumber of tries: \x1b[32m%d\x1b[0m  ", r);
	
	float HP_perc_float = (float)HP_perc / 100;
	
	long double a = (ratio[(number[0] * 100 + number[1] * 10 + number[2]) - 1] * bonusballvett[bonusindex] * status[statusindex]);
	a -= (0.666666) * (ratio[(number[0] * 100 + number[1] * 10 + number[2]) - 1] * bonusballvett[bonusindex] * status[statusindex]) * (HP_perc_float);

	long double b = 16711680 / a;
	b = sqrt(b);
	b = sqrt(b);
	b = 1048560 / b;
	long double p = ((b + 1) / 65536) * ((b + 1) / 65536) * ((b + 1) / 65536) * ((b + 1) / 65536);
	
	long double P;
	
	if (a >= 255)
		p = 1;
	
	else {
		long double temp = (b + 1) / 65536;
		for (int i = 0; i < 2; i++)
			temp *= temp;
		p = temp;
	}
	
	long double temp = (1 - p);
	for (int i = 0; i < r - 1; i++)
		temp = temp * (1 - p);
	
	P = 1 - temp;
	
	if (P > 1) 
		P = 1;
	
	printf("\n\nModified catch rate: \x1b[32m%Lf\x1b[0m   ", a);
	printf("\n\nProbability of capture tends to: \x1b[32m%Lf\x1b[0m%%      ", (P * 100));
}


void catchrate() {
	int ratio[DEX] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 120, 45, 255, 120, 45, 255, 120, 45, 255, 127, 255, 90, 255, 90, 190, 75, 255, 90, 235, 120, 45, 235, 120, 45, 150, 25, 190, 75, 170, 50, 255, 90, 255, 120, 45, 190, 75, 190, 75, 255, 50, 255, 90, 190, 75, 190, 75, 190, 75, 255, 120, 45, 200, 100, 50, 180, 90, 45, 255, 120, 45, 190, 60, 255, 120, 45, 190, 60, 190, 75, 190, 60, 45, 190, 45, 190, 75, 190, 75, 190, 60, 190, 90, 45, 45, 190, 75, 225, 60, 190, 60, 90, 45, 190, 75, 45, 45, 45, 190, 60, 120, 60, 30, 45, 45, 225, 75, 225, 60, 225, 60, 45, 45, 45, 45, 45, 45, 45, 225, 45, 45, 35, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 25, 3, 3, 3, 45, 45, 45, 3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 90, 255, 90, 255, 90, 255, 90, 90, 190, 75, 190, 150, 170, 190, 75, 190, 75, 235, 120, 45, 45, 190, 75, 65, 45, 255, 120, 45, 45, 235, 120, 75, 255, 90, 45, 45, 30, 70, 45, 225, 45, 60, 190, 75, 190, 60, 25, 190, 75, 45, 25, 190, 45, 60, 120, 60, 190, 75, 225, 75, 60, 190, 75, 45, 25, 25, 120, 45, 45, 120, 60, 45, 45, 45, 75, 45, 45, 45, 45, 45, 30, 3, 3, 3, 45, 45, 45, 3, 3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 127, 255, 90, 255, 120, 45, 120, 45, 255, 120, 45, 255, 120, 45, 200, 45, 190, 45, 235, 120, 45, 200, 75, 255, 90, 255, 120, 45, 255, 120, 45, 190, 120, 45, 180, 200, 150, 255, 255, 60, 45, 45, 180, 90, 45, 180, 90, 120, 45, 200, 200, 150, 150, 150, 225, 75, 225, 60, 125, 60, 255, 150, 90, 255, 60, 255, 255, 120, 45, 190, 60, 255, 45, 90, 90, 45, 45, 190, 75, 205, 155, 255, 90, 45, 45, 45, 45, 255, 60, 45, 200, 225, 45, 190, 90, 200, 45, 30, 125, 190, 75, 255, 120, 45, 255, 60, 60, 25, 225, 45, 45, 45, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 120, 45, 255, 127, 255, 45, 235, 120, 45, 255, 75, 45, 45, 45, 45, 120, 45, 45, 120, 45, 200, 190, 75, 190, 75, 190, 75, 45, 125, 60, 190, 60, 45, 30, 190, 75, 120, 225, 60, 255, 90, 255, 145, 130, 30, 100, 45, 45, 45, 50, 75, 45, 140, 60, 120, 45, 140, 75, 200, 190, 75, 25, 120, 60, 45, 30, 30, 30, 30, 30, 30, 30, 30, 45, 45, 30, 50, 30, 45, 60, 45, 75, 45, 3, 3, 3, 3, 3, 3, 3, 3, 3, 30, 3, 3, 45, 3, 3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 255, 255, 120, 45, 255, 90, 190, 75, 190, 75, 190, 75, 190, 75, 255, 120, 45, 190, 75, 255, 120, 45, 190, 45, 120, 60, 255, 180, 90, 45, 255, 120, 45, 45, 45, 255, 120, 45, 255, 120, 45, 190, 75, 190, 75, 25, 180, 90, 45, 120, 60, 255, 190, 75, 180, 90, 45, 190, 90, 45, 45, 45, 45, 190, 60, 75, 45, 255, 60,200, 100, 50, 200, 100, 50, 190, 45, 255, 120, 45, 190, 75, 200, 200, 75, 190, 75, 190, 60, 75, 190, 75, 255, 90, 130, 60, 30, 190, 60, 30, 255, 90, 190, 90, 45, 75, 60, 45, 120, 60, 25, 200, 75, 75, 180, 45, 45, 190, 90, 120, 45, 45, 190, 60, 190, 60, 90, 90, 45, 45, 45, 45, 15, 3, 3, 3, 3, 3 ,3, 3, 3, 3, 3, 3, 3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 127, 255, 120, 45, 255, 120, 45, 220, 65, 225, 120, 45, 200, 45, 220, 65, 160, 190, 75, 180, 90, 45, 200, 140, 200, 140, 190, 80, 120, 45, 225, 55, 225, 55, 190, 75, 45, 45, 45, 45, 45, 100, 180, 60, 45, 45, 45, 75, 120, 60, 120, 60, 190, 55, 190, 45, 45, 45, 3, 3, 3, 3};
	float bonusballvett[9] = {1, 1.5, 2, 3, 3.5, 4, 5, 8, 255};
	float status[4] = {1, 1.5, 2, 2.5};
	
	int posizione[1] = {0};
	char cursore[7] = {'^', ' ', ' ', ' ', ' ', ' ', ' '};
	
	int number[3] = {0, 0, 1};
	int HP_perc = 100;
	int bonusindex = 0;
	int statusindex = 0;
	int r = 1;
	
	printf("\x1b[2J");
	printf("\x1b[47;30m              Catch Rate  Calculator              \x1b[0m");
	printf("--------------------------------------------------");
	showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
	printCursoreC(cursore);
	
	int t_frame = 1;
	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		
		if (kDown & KEY_START)
			break;
		
		if ((kDown & KEY_DRIGHT) && (posizione[0] < 6)) {
			posCursore(cursore, posizione, 1);
			showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
			printCursoreC(cursore);
		}
		
		if ((kDown & KEY_DLEFT) && (posizione[0] > 0)) {
			posCursore(cursore, posizione, -1);
			showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
			printCursoreC(cursore);
		}
		
		if ((kDown & KEY_DUP) ^ (hidKeysHeld() & KEY_DUP && t_frame % DELAY == 1)) {
			if (posizione[0] == 0) {
				if (number[posizione[0]] < 7) {
					number[posizione[0]]++;
					check(number);
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 1 || posizione[0] == 2) {
				if (number[posizione[0]] < 9) {
					number[posizione[0]]++;
					check(number);
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 3) {
				if (HP_perc < 100) {
					HP_perc += 1;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
				else if (HP_perc == 100) {
					HP_perc = 1;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 4) {
				if (bonusindex < 8) {
					bonusindex++;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 5) {
				if (statusindex < 3) {
					statusindex++;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 6) {
				if (r < 200) {
					r++;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
		}
		
		if ((kDown & KEY_DDOWN) ^ (hidKeysHeld() & KEY_DDOWN && t_frame % DELAY == 1)) {
			if (posizione[0] == 0 || posizione[0] == 1 || posizione[0] == 2) {
				if (number[posizione[0]] > 0) {
					number[posizione[0]]--;
					check(number);
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 3) {
				if (HP_perc > 1) {
					HP_perc -= 1;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
				else if (HP_perc == 1) {
					HP_perc = 100;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);				
				}
			}
			if (posizione[0] == 4) {
				if (bonusindex > 0) {
					bonusindex--;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 5) {
				if (statusindex > 0) {
					statusindex--;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
			if (posizione[0] == 6) {
				if (r > 1) {
					r--;
					showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
				}
			}
		}
		
		if (kDown & KEY_SELECT) {
			posizione[0] = 0;
			cursore[0] = '^';
			for (int i = 1; i < 7; i++)
				cursore[i] = ' ';
			number[0] = 0;
			number[1] = 0;
			number[2] = 1;
			HP_perc = 100;
			bonusindex = 0;
			statusindex = 0;
			r = 1;
			printf("\x1b[2J");
			printf("\x1b[47;30m              Catch Rate  Calculator              \x1b[0m");
			printf("--------------------------------------------------");
			showC(number, ratio, HP_perc, bonusballvett, bonusindex, status, statusindex, r);
			printCursoreC(cursore);			
		}
		
		gfxFlushBuffers();
		gfxSwapBuffers();
		
		t_frame++;
		if (t_frame > 5000) t_frame = 1;
	}
}