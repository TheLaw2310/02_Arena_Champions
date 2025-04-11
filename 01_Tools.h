//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 01/29/2025                                                                   	 |
// Purpose.....: my tool box																     |
//________________________________________________________________________________________________
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define FLUSH flush()

#ifdef __WIN32
	#define PAUSE system("pause");
	#define CLS system("cls");
#endif

#ifdef __APPLE__
	#define PAUSE pause();
	#define CLS system("clear");
#endif

#ifdef __linux__
	#define PAUSE pause();
	#define CLS system("clear");
#endif

//________________________________________________________________________________________________

// PROTOTYPE HERE
void banner(char mess[], char sym);
void flush();
char getChar(char prompt[]);
double getDouble(char prompt[]);
int getInt(char prompt[]);
void getString(int size, char prompt[], char result[]);
void pause();
int randNum(int lower, int upper);

//________________________________________________________________________________________________

void banner(char mess[], char sym)
{
	int size = strlen(mess) + 6;
	printf("\n\t");
	for(int i = 0; i < size; i++)
		printf("%c", sym);
	printf("\n\t%c%c %s %c%c\n\t", sym, sym, mess, sym, sym);
	for(int i = 0; i < size; i++)
		printf("%c", sym);
	printf("\n\n");
} // end banner

//________________________________________________________________________________________________

void flush(){
	while(getchar() != '\n');
} // end Flush

//________________________________________________________________________________________________

char getChar(char prompt[]){
	char result = '\0';
	char badValue = 'Y';
	do{
		badValue = 'Y';
		printf("%s", prompt);
		if(scanf("%c", &result) == 1)
			badValue = 'N';
		else{
			badValue = 'Y';
			banner("I N V A L I D   I N P U T !", '*');
			PAUSE;
		}
		FLUSH;
	}while(badValue == 'Y');
	return result;
}// end getChar()

//________________________________________________________________________________________________

double getDouble(char prompt[])
{
	double result = 0;
	char badValue = 'Y';
	do{
		badValue = 'Y';
		printf("%s", prompt);
		if(scanf("%lf", &result) == 1)  // return a 1 then GOOD value    return a 0 the BAD value
			badValue = 'N';
		else{
			badValue = 'Y';
			banner("I N V A L I D   I N P U T ! ", '*');
			PAUSE;
		}// end if else
		FLUSH;
	}while(badValue == 'Y');
	return result;
} //end getNum

//________________________________________________________________________________________________

int getInt(char prompt[])
{
	int result = 0;
	char badValue = 'Y';
	do
	{
		badValue = 'Y';
		printf("%s", prompt);
		if(scanf("%i", &result) == 1)  // return a 1 then GOOD value    return a 0 the BAD value
			badValue = 'N';

		else{
			badValue = 'Y';
			banner("I N V A L I D   I N P U T ! ", '*');
			PAUSE;
		}// end if else
		FLUSH;
	}while(badValue == 'Y');
	return result;
} //end getInt

//________________________________________________________________________________________________

void getString(int size, char prompt[], char result[]){
	char input[size];
	printf("%s", prompt);
	scanf("%[^\n]", input); FLUSH;
	strcpy(result, input);
}// end getString()

//________________________________________________________________________________________________

void pause(){
    printf("\n\nPress ENTER to continue...");
    getchar();
}//end of pause()

//________________________________________________________________________________________________

int randNum(int lower, int upper)
{
	int result = lower + rand() % (upper - lower + 1);
	return result;
}//end randNum

//________________________________________________________________________________________________