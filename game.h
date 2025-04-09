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
void displayBattle(BATTLE* battle);
void displayBattleHistory(BATTLE** battles, int eSize);
void displayPlayerStats(PLAYER* player);
int enemyTurn(PLAYER* player, ENEMY* enemy);
BATTLE* findBattle(BATTLE** battles, int eSize, int input);
int findBattleIndex(BATTLE** battles, int eSize, int input);
void freeMemory(BATTLE** battles, int eSize);
void gameLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size, int* eSize);
void healPlayer(PLAYER* player);
void initializeEnemyStats(ENEMY* enemy);
void initializePlayerStats(PLAYER* player, ENEMY* enemy);
int mainMenu();
void mainMenuLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size, int* eSize);
int playerTurn(PLAYER* player, ENEMY* enemy);
void recordBattle(PLAYER* player, ENEMY* enemy, BATTLE* battle, int outcome, int turn, int damageDealt, int damageTaken);
void removeBattle(BATTLE** battles, int* eSize);
void searchBattleByID(BATTLE** battles, int eSize);
void sortByRounds(BATTLE** battles, int eSize);
void sortChronologically(BATTLE** battles, int eSize);
void sortDamageIn(BATTLE** battles, int eSize);
void sortDamageOut(BATTLE** battles, int eSize);
void sortingMenu(BATTLE** battles, int eSize);

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
        banner("C O M B A T ", '=');
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

void displayBattle(BATTLE* battle){
    CLS;
    banner("FOUND BATTLE", '=');
    printf("%-4s %-20s %-15s %-15s %-15s %-15s\n", "#", "ENEMY", "RESULT", "DMG-OUT", "DMG-IN", "#ROUNDS");
    printf("\n%-4i %-20s %-15i %-15i %-15i %-15i", battle->ID, battle->enemyType, battle->result, battle->damageDealt, battle->damageTaken, battle->roundCount);
    PAUSE;
}//end displayBattle()

//_________________________________________________________________________________________________

void displayBattleHistory(BATTLE** battles, int eSize){
    if(eSize <= 0){
        CLS;
        banner("GO FIGHT SOMETHING AND COMEBACK", '*');
        PAUSE;
    }
    else{
        CLS;
        banner("B A T T L E   H I S T O R Y", '=');
        printf("%-4s %-20s %-15s %-15s %-15s %-15s\n", "#", "ENEMY", "RESULT", "DMG-OUT", "DMG-IN", "#ROUNDS");
        for(int i = 0; i < eSize; i++){
            if (battles[i] != NULL)
                printf("\n%-4i %-20s %-15i %-15i %-15i %-15i", battles[i]->ID, battles[i]->enemyType, battles[i]->result, battles[i]->damageDealt, battles[i]->damageTaken, battles[i]->roundCount);
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

BATTLE* findBattle(BATTLE** battles, int eSize, int input){
    BATTLE* result = NULL;

    for(int i = 0; i < eSize; i++)
        if(input == battles[i]->ID)
            result = battles[i];
    return result;
}//end findBattle()

//________________________________________________________________________________________________

int findBattleIndex(BATTLE** battles, int eSize, int input){
    int i;
    for(i = 0; i < eSize; i++)
        if(input == battles[i]->ID)
            return i;
    
    return -1;
}//end findBattle()

//________________________________________________________________________________________________

void freeMemory(BATTLE** battles, int eSize){
    for(int i = eSize - 1; i >= 0; i--)
        free(battles[i]);
    free(battles);
}//end freeMemory()

//________________________________________________________________________________________________

void gameLoop(PLAYER* player, ENEMY* enemy, BATTLE** battles, int size,  int* eSize){
    if(player->health <= 0){
        CLS;
        banner("YOU'RE DEAD... HEAL YOUR CHARATER AND COME BACK", '*');
        PAUSE;
        return;
    }

    int outcome, turn, damageDealt, damageTaken;

    //Starting a new battle
    BATTLE* currentBattle = createNewBattle(size, *eSize);
    battles[*eSize] = currentBattle;
    (*eSize)++;
    battles[*eSize - 1]->ID = *eSize;   //keep track of chronological order

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
                sortingMenu(battles, *eSize);
                displayBattleHistory(battles, *eSize);
                break;
            case 5:
                searchBattleByID(battles, *eSize);
                break;
            case 6:
                removeBattle(battles, eSize);
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

void removeBattle(BATTLE** battles, int* eSize){
    CLS;
    banner("REMOVE BATTLE", '=');
    int input = getInt("Enter the battle # you would like to delete: ");
    int battleIndex = findBattleIndex(battles, *eSize, input);

    if(battleIndex >= 0 && battleIndex < *eSize){
        free(battles[battleIndex]);

        //shift following nodes back by one
        for(int i = battleIndex; i < *eSize - 1; i++)
            battles[i] = battles[i + 1];
        
        (*eSize)--;
    }//end if()
    CLS;
    banner("BATTLE HAS BEEN REMOVE FROM HISTORY", '=');
    PAUSE;
}//end removeBattle()

//________________________________________________________________________________________________

void searchBattleByID(BATTLE** battles, int eSize){
    CLS;
    banner("SEARCHING FOR BATTLE", '=');
    int input = getInt("Enter the Battle # you'd like to search for: ");
    BATTLE* battle = findBattle(battles, eSize, input);
    displayBattle(battle);
}//end searchBattleByID()

//________________________________________________________________________________________________

void sortByRounds(BATTLE** battles, int eSize){
    BATTLE* temp = NULL;
    for(int i = 0; i < eSize - 1; i++){
        for(int j = 0; j < eSize - 1; j++){
            if(battles[j]->roundCount > battles[j + 1]->roundCount){
                temp = battles[j];
                battles[j] = battles[j + 1];
                battles[j + 1] = temp;
            }//swap if no in order
        }
    }
}//end sortByRounds()

//________________________________________________________________________________________________

void sortChronologically(BATTLE** battles, int eSize){
    BATTLE* temp = NULL;
    for(int i = 0; i < eSize - 1; i++){
        for(int j = 0; j < eSize - 1; j++){
            if(battles[j]->ID > battles[j + 1]->ID){
                temp = battles[j];
                battles[j] = battles[j + 1];
                battles[j + 1] = temp;
            }//swap if no in order
        }
    }
}//end sortChronologically()

//________________________________________________________________________________________________

void sortDamageIn(BATTLE** battles, int eSize){
    BATTLE* temp = NULL;
    for(int i = 0; i < eSize - 1; i++){
        for(int j = 0; j < eSize - 1; j++){
            if(battles[j]->damageTaken > battles[j + 1]->damageTaken){
                temp = battles[j];
                battles[j] = battles[j + 1];
                battles[j + 1] = temp;
            }//swap if no in order
        }
    }
}//end sortDamageIn()

//________________________________________________________________________________________________

void sortDamageOut(BATTLE** battles, int eSize){
    BATTLE* temp = NULL;
    for(int i = 0; i < eSize - 1; i++){
        for(int j = 0; j < eSize - 1; j++){
            if(battles[j]->damageDealt > battles[j + 1]->damageDealt){
                temp = battles[j];
                battles[j] = battles[j + 1];
                battles[j + 1] = temp;
            }//swap if no in order
        }
    }
}//end sortDamageOut()

//________________________________________________________________________________________________

void sortingMenu(BATTLE** battles, int eSize){
    CLS;
    banner("A R E N A   C H A M P I O N S", '=');
    printf("-) Start Game\n");
    printf("-) Player Stats\n");
    printf("-) Heal Player\n");
    printf("-) View Battle History\n");
    printf("\t1) by chronological order\n");
    printf("\t2) by DMG-OUT low-high\n");
    printf("\t3) by DMG-IN low-high\n");
    printf("\t4) by # of rounds low-high\n");
    printf("-) Search Battle History\n");
    printf("-) Delete Battle History\n");
    printf("-) Exit\n");
    int choice = getInt("\nEnter selection from the menu: ");
    
    switch(choice){
        case 1:
            sortChronologically(battles, eSize);
            break;
        case 2:
            sortDamageOut(battles, eSize);
            break;
        case 3:
            sortDamageIn(battles, eSize);
            break;
        case 4:
            sortByRounds(battles, eSize);
            break;
        default:
            CLS;
            banner("I N V A L I D   I N P U T", '*');
            PAUSE;
    }//end switch
}//end sortingMenu()

//________________________________________________________________________________________________