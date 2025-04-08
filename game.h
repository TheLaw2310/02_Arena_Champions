//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 04/08/2025                                                                   	 |
// Purpose.....: Arena Champions Header 													     |
//________________________________________________________________________________________________
#pragma once

#include "01_Tools.h"
#include "structs.h"

//________________________________________________________________________________________________

// PROTOTYPES
int mainMenu();
void mainMenuLoop();

//________________________________________________________________________________________________

int mainMenu(){
    CLS;
    banner("A R E N A   C H A M P I O N S", '=');
    printf("1) Start Game\n");
    printf("2) Exit\n");
    int choice = getInt("\nEnter selection from the menu: ");
    return choice;
}
//_________________________________________________________________________________________________

void mainMenuLoop(){
    int choice = 0;
    do{
        choice = mainMenu();
        switch(choice){
            case 1:
                // Start game logic here
                break;
            case 2:
                CLS;
                banner("Goodbye!", '=');
                break;
            default:
                CLS;
                banner("I N V A L I D   I N P U T ! ", '*');
                PAUSE;
                break;
        }
    }while(choice != 0);
}