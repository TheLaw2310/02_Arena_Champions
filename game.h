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
char* battleResult(PLAYER* player, ENEMY* enemy);
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
void gameLoop(PLAYER* player, BATTLE** battles, int size, int* eSize);
void healPlayer(PLAYER* player);
ENEMY initializeEnemyStats();
void initializePlayerStats(PLAYER* player);
int mainMenu();
void mainMenuLoop(PLAYER* player, BATTLE** battles, int size, int* eSize);
int playerTurn(PLAYER* player, ENEMY* enemy);
void recordBattle(PLAYER* player, ENEMY enemy, BATTLE* battle, char outcome[], int turn, int damageDealt, int damageTaken);
void removeBattle(BATTLE** battles, int* eSize);
void searchBattleByID(BATTLE** battles, int eSize);
void sortByRounds(BATTLE** battles, int eSize);
void sortChronologically(BATTLE** battles, int eSize);
void sortDamageIn(BATTLE** battles, int eSize);
void sortDamageOut(BATTLE** battles, int eSize);
void sortingMenu(BATTLE** battles, int eSize);

//________________________________________________________________________________________________

char* battleResult(PLAYER* player, ENEMY* enemy){
    if(player->health <= 0 && enemy->health > 0){
        player->losses++;
        player->health = 0;
        CLS;
        banner("P L A Y E R   L O S T", '*');
        return "LOST";
    }
    else if(player->health > 0 && enemy->health <= 0){
        player->wins++;
        CLS;
        banner("P L A Y E R   W O N", '=');
        return "WON";
    }
    else{
        CLS;
        banner("I T   W A S   A   T I E", '*');
        return "TIED";
    }
}//end battleResult()

//________________________________________________________________________________________________

void combatLoop(PLAYER* player, ENEMY* enemy, int* round, int* damageDealt, int* damageTaken){
    *round = 0;
    CLS;
    banner("C O M B A T   B E G I N S", '=');
    printf("!! A wild %s has a appeard !!\n", enemy->type);
    printf("STATS--> HP: %i | ATK: %i | DEF: %i", enemy->maxHealth, enemy->damage, enemy->defense);
    PAUSE;
    while(player->health > 0 && enemy->health > 0){ //each loop is a round
        (*round)++;
        CLS;
        printf("---- R O U N D %i ----\n\n", *round);
        printf("%s: HP: %i | ATK: %i | DEF: %i\n", player->name, player->health, player->damage, player->defense);
        printf("%s: HP: %i | ATK: %i | DEF: %i\n\n", enemy->type, enemy->health, enemy->damage, enemy->defense);
        *damageDealt = playerTurn(player, enemy);
        *damageTaken = enemyTurn(player, enemy);
        
        //reset player defense
        player->defense = 5;

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
    printf("\n%-4i %-20s %-15s %-15i %-15i %-15i", battle->ID, battle->enemyType, battle->result, battle->damageDealt, battle->damageTaken, battle->roundCount);
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
                printf("\n%-4i %-20s %-15s %-15i %-15i %-15i", battles[i]->ID, battles[i]->enemyType, battles[i]->result, battles[i]->damageDealt, battles[i]->damageTaken, battles[i]->roundCount);
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
    int damageTaken = enemy->damage - player->defense;
    
    //Don't allow defense to heal player
    if(damageTaken < 0)
        damageTaken = 0;

    player->health -= damageTaken;

    printf("%s Struck for %i DMG", enemy->type, damageTaken);
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

void gameLoop(PLAYER* player, BATTLE** battles, int size,  int* eSize){
    if(player->health <= 0){
        CLS;
        banner("YOU'RE DEAD... HEAL YOUR CHARATER AND COME BACK", '*');
        PAUSE;
        return;
    }

    int round, damageDealt, damageTaken;

    //Starting a new battle and adding to array of battles
    BATTLE* currentBattle = createNewBattle(size, *eSize);
    battles[*eSize] = currentBattle;
    (*eSize)++;

    //keep track of chronological order of battles
    battles[*eSize - 1]->ID = *eSize;

    //Spawning enemy
    ENEMY enemy = initializeEnemyStats();
    
    //main combat loop
    combatLoop(player, &enemy, &round, &damageDealt, &damageTaken);
    
    //display win lose tie message
    char* outcome = battleResult(player, &enemy); // 1 = lose, 2 = win, 3 = tie
    
    //save battle stats 
    recordBattle(player, enemy, currentBattle, outcome, round, damageDealt, damageTaken);
}//end gameLoop()

//_________________________________________________________________________________________________

void healPlayer(PLAYER* player){
    CLS;
    if(player->health >= player->maxHealth){
        banner("UNABLE TO HEAL PLAYER AT MAX HEALTH", '*');
        PAUSE;
    }//end if(health is already full)
    else{
        player->health = player->maxHealth;
        banner("PLAYER IS HEALED TO MAX HEALTH", '=');
        PAUSE;
    }//end if(heal successful)
}//end healPlayer()

//_________________________________________________________________________________________________

ENEMY initializeEnemyStats(){
    ENEMY result;
    //initialize enemy stats
    strcpy(result.type, "Troll");
    result.maxHealth = 75;
    result.health = result.maxHealth;
    result.damage = 8;
    result.defense = 6;

    return result;
}//end initializeEnemyStats()

//_________________________________________________________________________________________________

void initializePlayerStats(PLAYER* player){
    CLS;
    banner("Choose your character", '=');
    getString(100, "Enter your character name: ", player->name);
    player->maxHealth = 100;
    player->health = player->maxHealth;
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

void mainMenuLoop(PLAYER* player, BATTLE** battles, int size, int* eSize){
    int choice = 0;
    do{
        choice = mainMenu();
        switch(choice){
            case 1:
                gameLoop(player, battles, size, eSize);
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
    printf("Choose an action:\n");
    printf("1) Attack\n");
    printf("2) Defend (2x DEF)\n");
    int action = getInt("\n>>>");
    while(action != 1 && action != 2){
        printf("\nInvalid input!!\n");
        action = getInt("\n>>>");
    }
    //contains calculated damage after accounting for enemy defense
    int damageDealt = 0;

    if(action == 1){
        damageDealt = player->damage - enemy->defense;
        enemy->health -= damageDealt;
        printf("\n%s struck for %i DMG\n", player->name, damageDealt);
        return damageDealt;
    }
    else if(action == 2){
        player->defense *= 2;
        printf("%s raised DEF to %i\n", player->name, player->defense);
        return damageDealt;
    }
    else{
        printf("\nFailed to commit any action.\n");
        return damageDealt;
    }
}//end playerTurn()

//________________________________________________________________________________________________

void recordBattle(PLAYER* player, ENEMY enemy, BATTLE* battle, char outcome[], int round, int damageDealt, int damageTaken){
    strcpy(battle->enemyType, enemy.type);
    strcpy(battle->result, outcome);
    battle->roundCount = round;
    battle->damageDealt = damageDealt;
    battle->damageTaken = damageTaken;
    
    printf("\nBattle data has been saved");
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