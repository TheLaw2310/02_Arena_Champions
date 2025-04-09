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
int battleResult(PLAYER* player, ENEMY* enemy);
void combatLoop(PLAYER* player, ENEMY* enemy, int* turn, int* damageDealt, int*damageTaken);
BATTLE** createBattleArray(int size);
BATTLE* createNewBattle(int size, int eSize);
void displayBattleHistory(BATTLE** battles, int eSize);
void displayPlayerStats(PLAYER* player);
int enemyTurn(PLAYER* player, ENEMY* enemy);
void freeMemory(BATTLE** battles, int eSize);
void gameLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size, int* eSize);
void healPlayer(PLAYER* player);
void initializeEnemyStats(ENEMY* enemy);
void initializePlayerStats(PLAYER* player, ENEMY* enemy);
int mainMenu();
void mainMenuLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size, int* eSize);
int playerTurn(PLAYER* player, ENEMY* enemy);
void recordBattle(PLAYER* player, ENEMY* enemy, BATTLE* battle, int outcome, int turn, int damageDealt, int damageTaken);

//________________________________________________________________________________________________

int battleResult(PLAYER* player, ENEMY* enemy){
    if(player->health <= 0 && enemy->health > 0){
        player->losses++;
        CLS;
        banner("G A M E   O V E R", '*');
        PAUSE;
        return 1;
    }
    else if(player->health > 0 && enemy->health <= 0){
        player->wins++;
        CLS;
        banner("P L A Y E R   W I N", '=');
        PAUSE;
        return 2;
    }
    else{
        CLS;
        banner("IT'S A TIE... DIDN'T SEE THAT ONE COMING", '*');
        PAUSE;
        return 3;
    }
}//end battleResult()

//________________________________________________________________________________________________

void combatLoop(PLAYER* player, ENEMY* enemy, int* turn, int* damageDealt, int* damageTaken){
    *turn = 1;
    while(player->health > 0 && enemy->health > 0){
        CLS;
        printf("Turn (%i)\n\n", *turn);
        (*turn)++;

        *damageDealt = playerTurn(player, enemy);
        *damageTaken = enemyTurn(player, enemy);
        
        printf("Player dealt %d damage to enemy and received %d damage!", *damageDealt, *damageTaken);
        PAUSE;
    }//end combat loop()
}//end combatLoop()

//________________________________________________________________________________________________

BATTLE** createBattleArray(int size){
    BATTLE** battle = (BATTLE**)calloc(size, sizeof(BATTLE*));
    if(battle == NULL){
        CLS;
        banner("Memory allocation failed!", '*');
        PAUSE;
        exit(-1);
    }
    return battle;
}// end createBattleArray()

//_________________________________________________________________________________________________

BATTLE* createNewBattle(int size, int eSize){
    if(eSize < size){
        BATTLE* newBattle = (BATTLE*)malloc(sizeof(BATTLE));
        if(newBattle == NULL){
            CLS;
            banner("FAILED TO ALLOCATE MEMORY FOR NEW BATTLE", '*');
            PAUSE;
            exit(-1);
        }//end if(failed)
        return newBattle;
    }//end if(have space)
    else{
        CLS;
        banner("YOU HAVE FOUGHT ALL YOU BATTLES... RESTART THE GAME IF YOU'D LIKE TO FIGHT AGAIN", '*');
        PAUSE;
        exit(-1);
    }//end if(out of space)
}//end createNewBattle()

//_________________________________________________________________________________________________

void displayBattleHistory(BATTLE** battles, int eSize){
    CLS;
    banner("B A T T L E   H I S T O R Y", '=');
    if(eSize <= 0){
        CLS;
        banner("GO FIGHT SOMETHING AND COMEBACK", '*');
        PAUSE;
    }
    else{
        printf("%-4s %-20s %-15s %-15s %-15s %-15s\n", "#", "ENEMY", "RESULT", "DMG-OUT", "DMG-IN", "#ROUNDS");
        for(int i = 0; i < eSize; i++){
            if (battles[i] != NULL)
                printf("\n%-4i %-20s %-15i %-15i %-15i %-15i", i + 1, battles[i]->enemyType, battles[i]->result, battles[i]->damageDealt, battles[i]->damageTaken, battles[i]->roundCount);
            else
                printf("\n%4i %15s", i + 1, "INVALID BATTLE");
        }//end for(i)
        PAUSE;
    }
}//end displayBattleHistory()

//_________________________________________________________________________________________________

void displayPlayerStats(PLAYER* player){
    CLS;
    banner("P L A Y E R   S T A T S", '=');
    printf("Name    :\t %s\n", player->name);
    printf("Health  :\t %d\n", player->health);
    printf("Damage  :\t %d\n", player->damage);
    printf("Defense :\t %d\n", player->defense);
    printf("Wins    :\t %d\n", player->wins);
    printf("Losses  :\t %d\n", player->losses);
    PAUSE;
}//end displayPlayerStats()

//_________________________________________________________________________________________________

int enemyTurn(PLAYER* player, ENEMY* enemy){
    int damageTaken = 0;
    damageTaken = enemy->damage - player->defense;
    player->health -= damageTaken;
    return damageTaken;
}//end enemyTurn()

//________________________________________________________________________________________________

void freeMemory(BATTLE** battles, int eSize){
    for(int i = eSize - 1; i >= 0; i--)
        free(battles[i]);
    free(battles);
}//end freeMemory()

//________________________________________________________________________________________________

void gameLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size,  int* eSize){
    int outcome, turn, damageDealt, damageTaken;
    CLS;
    banner("C O M B A T ", '=');
    
    //Starting a new battle
    BATTLE* currentBattle = createNewBattle(size, *eSize);
    battles[*eSize] = currentBattle;
    (*eSize)++;

    //Spawning enemy(s)
    initializeEnemyStats(enemy);
    
    combatLoop(player, enemy, &turn, &damageDealt, &damageTaken);
    
    outcome = battleResult(player, enemy); // 1 = lose, 2 = win, 3 = tie
    
    recordBattle(player, enemy, currentBattle, outcome, turn, damageDealt, damageTaken);
}//end gameLoop()

//_________________________________________________________________________________________________

void healPlayer(PLAYER* player){
    CLS;
    if(player->health >= 100){
        banner("UNABLE TO HEAL PLAYER AT MAX HEALTH", '*');
        PAUSE;
    }//end if(health is already full)
    else{
        player->health = 100;
        banner("PLAYER IS HEALED TO MAX HEALTH", '=');
        PAUSE;
    }//end if(heal successful)
}//end healPlayer()

//_________________________________________________________________________________________________

void initializeEnemyStats(ENEMY* enemy){
    //initialize enemy stats
    strcpy(enemy->type, "Troll");
    enemy->health = 75;
    enemy->damage = 8;
    enemy->defense = 6;
}//end initializeEnemyStats()

//_________________________________________________________________________________________________

void initializePlayerStats(PLAYER* player, ENEMY* enemy){
    CLS;
    banner("Choose your character", '=');
    getString(100, "Enter your character name: ", player->name);
    player->health = 100;
    player->damage = 10;
    player->defense = 5;
    player->wins = 0;
    player->losses = 0;
}//end initializeGame()

//_________________________________________________________________________________________________

int mainMenu(){
    CLS;
    banner("A R E N A   C H A M P I O N S", '=');
    printf("1) Start Game\n");
    printf("2) Player Stats\n");
    printf("3) Heal Player\n");
    printf("4) View Battle History\n");
    printf("5) Search Battle History\n");
    printf("6) Delete Battle History\n");
    printf("7) Exit\n");
    int choice = getInt("\nEnter selection from the menu: ");
    return choice;
}//end mainMenu()

//_________________________________________________________________________________________________

void mainMenuLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size, int* eSize){
    int choice = 0;
    do{
        choice = mainMenu();
        switch(choice){
            case 1:
                gameLoop(player, enemy, battles, size, eSize);
                break;
            case 2:
                displayPlayerStats(player);
                break;
            case 3:
                healPlayer(player);
                break;
            case 4:
                displayBattleHistory(battles, *eSize);
                break;
            case 5:
                //search battle history
                break;
            case 6:
                //delete battle history
                break;  
            case 7:
                freeMemory(battles, *eSize);
                CLS;
                banner("Goodbye!", '=');
                break;
            default:
                CLS;
                banner("I N V A L I D   I N P U T ! ", '*');
                PAUSE;
                break;
        }
    }while(choice != 7);
}//end mainMenuLoop()

//________________________________________________________________________________________________

int playerTurn(PLAYER* player, ENEMY* enemy){
    int damageDealt = 0;
    damageDealt = player->damage - enemy->defense;
    enemy->health -= damageDealt;
    return damageDealt;
}//end playerTurn()

//________________________________________________________________________________________________

void recordBattle(PLAYER* player, ENEMY* enemy, BATTLE* battle, int outcome, int turn, int damageDealt, int damageTaken){
    strcpy(battle->enemyType, enemy->type);
    battle->result = outcome;
    battle->roundCount = turn;
    battle->damageDealt = damageDealt;
    battle->damageTaken = damageTaken;
    
    CLS;
    banner("BATTLE HAS BEEN SAVED", '=');
    PAUSE;
}//end recordBattle()

//________________________________________________________________________________________________