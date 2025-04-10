//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 04/08/2025                                                                   	 |
// Purpose.....: Arena Champions Game														     |
//________________________________________________________________________________________________

#include "01_Tools.h"
#include "structs.h"
#include "game.h"

//________________________________________________________________________________________________

int main(){
    srand((unsigned) time(NULL));
    
    CLS;
    banner("W E L C O M E", '=');
    int size = getInt("Enter how many battles you want take part in: ");
    int eSize = 0;

    BATTLE** battles = createBattleArray(size);
    if(battles !=NULL){
        PLAYER player;
        initializePlayerStats(&player);
        mainMenuLoop(&player, battles, size, &eSize);
    }
    else{
        CLS;
        banner("FATAL ERROR: Memory allocation failed!", '*');
        PAUSE;
        exit(-1);
    }
    return 0;   
}//end main()

//________________________________________________________________________________________________