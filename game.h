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
void enemyTurn(PLAYER* player, ENEMY* enemy, int* damageGiven, int* damageTaken);
BATTLE* findBattle(BATTLE** battles, int eSize, int input);
int findBattleIndex(BATTLE** battles, int eSize, int input);
void freeMemory(BATTLE** battles, int eSize);
void gameLoop(PLAYER* player, BATTLE** battles, int size, int* eSize);
void healPlayer(PLAYER* player);
ENEMY initializeEnemyStats();
void initializePlayerStats(PLAYER* player);
int mainMenu();
void mainMenuLoop(PLAYER* player, BATTLE** battles, int size, int* eSize);
void playerTurn(PLAYER* player, ENEMY* enemy, int* damageDealt, int* damageTaken);
void recordBattle(PLAYER* player, ENEMY enemy, BATTLE* battle, char outcome[], int turn, int damageDealt, int damageTaken);
void removeBattle(BATTLE** battles, int* eSize);
void roundHeader(PLAYER* player, ENEMY* enemy, int round);
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
    int enemyTurns = enemy->speed;
    CLS;
    banner("C O M B A T   B E G I N S", '=');
    printf("!! A wild %s has a appeard !!\n", enemy->type);
    printf("STATS--> HP: %i | ATK: %i | DEF: %i | SPEED: %i", enemy->health, enemy->damage, enemy->defense, enemy->speed);
    PAUSE;

    //each loop is a round
    while(player->health > 0 && enemy->health > 0){ //each loop is a round
        (*round)++;
        //each loop is a player turn
        for(int i = 0; i < player->speed; i++){
            //if Giant stunned player
            if(enemy->ID == 8 && randNum(0,1) == 0){
                roundHeader(player, enemy, *round);
                printf("\n%s turn -%i-", player->name, i+1);
                printf("\n\n%s Stunned %s and skipped this turn\n", enemy->type, player->name);
                printf("________________________________________________\n");
                PAUSE;
            }
            
            //normal turn    
            else{    
                roundHeader(player, enemy, *round);
                printf("\n%s turn -%i-", player->name, i+1);
                
                playerTurn(player, enemy, damageDealt, damageTaken);
                
                if(player->isDefending == 1){
                    enemyTurn(player, enemy, damageDealt, damageTaken);
                    enemyTurns--;
                    player->isDefending = 0;
                    player->defense = player->baseDefense;
                }
                //exit loop if enemy is defeated
                if(enemy->health <= 0)
                    i = player->speed;
                
                printf("________________________________________________\n");
                PAUSE;
            }
        }

        if(enemy->health > 0){
            //each loop is an enemy turn
            for(int i = 0; i < enemyTurns; i++){
                roundHeader(player, enemy, *round);
                printf("\n%s turn -%i-\n", enemy->type, i+1);
                enemyTurn(player, enemy, damageDealt, damageTaken);
                printf("________________________________________________\n");
                PAUSE;
            }
        }
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
    printf("Defense :\t %d\n", player->baseDefense);
    printf("Speed   :\t %d\n", player->speed);
    printf("Wins    :\t %d\n", player->wins);
    printf("Losses  :\t %d\n", player->losses);
    PAUSE;
}//end displayPlayerStats()

//_________________________________________________________________________________________________

void enemyTurn(PLAYER* player, ENEMY* enemy, int* damageDealt, int* damageTaken){
    switch(enemy->ID){
        case 1:
            if(randNum(0,3) == 0){
                enemy->damage *= 2;
                printf("\n%s has achieved a *Critical Hit* and will deal %i Damage\n", enemy->type, enemy->damage);
            }
            break;
        case 2:
            player->defense = 0;
            printf("\n%s has bypassed the player Defense!!\n", enemy->type);
            break;
        default:
            break;
    }//end switch()
    
    //execute the code in if statement 33% of the time that player is defending
    if(player->isDefending == 1 && randNum(0,2) == 0){
        *damageDealt = ceil(0.75 * enemy->damage);
        enemy->health -= *damageDealt;

        printf("\n%s Parried %s's attack and lost 0 HP!!\n", player->name, enemy->type);
        printf("%s took %i damage\n", enemy->type, *damageDealt);
    }//if Perry was successfull
    else{
        *damageTaken = enemy->damage - player->defense;
        
        //Don't allow defense to heal player
        if(*damageTaken < 0)
            *damageTaken = 0;

        player->health -= *damageTaken;

        //prevent overkill (negative HP)
        if(player->health <= 0)
            player->health = 0;

        printf("\n%s Struck for %i DMG\n", enemy->type, *damageTaken);
        
        if(enemy->ID == 6 && *damageTaken > 0){
            enemy->health += 2;
            printf("\n%s Recovered 2 HP with the successful hit\n", enemy->type);
        }//end if(vampire and successful hit)
    }
    enemy->damage = enemy->baseDamage;
    player->defense = player->baseDefense;
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
    ENEMY enemies[10];

    strcpy(enemies[0].type, "Zombie");
    enemies[0].ID = 0;
    enemies[0].health = 35;
    enemies[0].baseDamage = 4;
    enemies[0].damage = enemies[0].baseDamage;
    enemies[0].defense = 0;
    enemies[0].speed = 1;

    strcpy(enemies[1].type, "Werewolf");
    enemies[1].ID = 1;
    enemies[1].health = 20;
    enemies[1].baseDamage = 8;
    enemies[1].damage = enemies[1].baseDamage;
    enemies[1].defense = 2;
    enemies[1].speed = 5;
    //25% chance to land 2x damage (DONE)
    
    strcpy(enemies[2].type, "Sorcerer");
    enemies[2].ID = 2;
    enemies[2].health = 25;
    enemies[2].baseDamage = 9;
    enemies[2].damage = enemies[2].baseDamage;
    enemies[2].defense = 3;
    enemies[2].speed = 3;
    //Ignores defense (DONE)

    strcpy(enemies[3].type, "Knight");
    enemies[3].ID = 3;
    enemies[3].health = 30;
    enemies[3].baseDamage = 5;
    enemies[3].damage = enemies[3].baseDamage;
    enemies[3].defense = 7;
    enemies[3].speed = 2;
    
    strcpy(enemies[4].type, "Imp");
    enemies[4].ID = 4;
    enemies[4].health = 18;
    enemies[4].baseDamage = 6;
    enemies[4].damage = enemies[4].baseDamage;
    enemies[4].defense = 1;
    enemies[4].speed = 4;
    //DOT (2 damage per turn for 3 turns starting from and after initial hit)
    
    strcpy(enemies[5].type, "Scorpion");
    enemies[5].ID = 5;
    enemies[5].health = 22;
    enemies[5].baseDamage = 7;
    enemies[5].damage = enemies[5].baseDamage;
    enemies[5].defense = 3;
    enemies[5].speed = 3;
    //30% Poison (1-3 damage/turn for 5 turns)
    
    strcpy(enemies[6].type, "Vampire");
    enemies[6].ID = 6;
    enemies[6].health = 28;
    enemies[6].baseDamage = 6;
    enemies[6].damage = enemies[6].baseDamage;
    enemies[6].defense = 4;
    enemies[6].speed = 4;
    //Heals 2HP everytime it lands a hit > 0 DMG (DONE)
    
    strcpy(enemies[7].type, "Assassin");
    enemies[7].ID = 7;
    enemies[7].health = 15;
    enemies[7].baseDamage = 10;
    enemies[7].damage = enemies[7].baseDamage;
    enemies[7].defense = 1;
    enemies[7].speed = 6;
    //50% chance to not take any damage (DONE)
    
    strcpy(enemies[8].type, "Giant");
    enemies[8].ID = 8;
    enemies[8].health = 35;
    enemies[8].baseDamage = 7;
    enemies[8].damage = enemies[8].baseDamage;
    enemies[8].defense = 5;
    enemies[8].speed = 2;
    //50% chance to skip player turn (STUN) (DONE)
    
    strcpy(enemies[9].type, "Dragon");
    enemies[9].ID = 9;
    enemies[9].health = 50;
    enemies[9].baseDamage = 10;
    enemies[9].damage = enemies[9].baseDamage;
    enemies[9].defense = 6;
    enemies[9].speed = 4;
    //Every 3rd turn, dragon breathes fire (2x damage) and heals 10HP or (12 damage ignoring defense)

    int randomEnemy = randNum(0, 9);
    ENEMY enemy = enemies[randomEnemy];
    return enemy;
}//end initializeEnemyStats()

//_________________________________________________________________________________________________

void initializePlayerStats(PLAYER* player){
    CLS;
    banner("Choose your character", '=');
    getString(100, "Enter your character name: ", player->name);
    player->maxHealth = 40;
    player->health = player->maxHealth;
    player->damage = 8;
    player->baseDefense = 5;
    player->defense = player->baseDefense;
    player->speed = 3;
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

void playerTurn(PLAYER* player, ENEMY* enemy, int* damageDealt, int* damageTaken){
    printf("\n\nChoose an action:\n");
    printf("1) Attack\n");
    printf("2) Defend (2x DEF)\n");
    int action = getInt("\n>>>");
    
    //check for input error
    while(action != 1 && action != 2){
        printf("\nInvalid input!!\n");
        action = getInt("\n>>>");
    }

    //contains calculated damage after accounting for enemy defense
    if(action == 1){
        if(enemy->ID == 7 && randNum(0,1) == 0){
            player->isDefending = 0;
            *damageDealt = 0;
            printf("\n%s Evaded %s's attack and received 0 damage!\n", enemy->type, player->name);
        }
        else{
            player->isDefending = 0;
            *damageDealt = player->damage - enemy->defense;
            enemy->health -= *damageDealt;
            printf("\n%s Struck for %i DMG\n", player->name, *damageDealt);
        }
    }
    else if(action == 2){
        player->isDefending = 1;
        player->defense = player->baseDefense + (player->baseDefense * 0.5);
        printf("\n%s Raised DEF to %i\n", player->name, player->defense);
    }
    else{
        printf("\nFailed to commit any action.\n");
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

void roundHeader(PLAYER* player, ENEMY* enemy, int round){
    CLS;
    printf("---- R O U N D %i ----\n\n", round);
    printf("%s: HP: %i | ATK: %i | DEF: %i | SPEED: %i\n", player->name, player->health, player->damage, player->defense, player->speed);
    printf("%s: HP: %i | ATK: %i | DEF: %i | SPEED: %i\n\n", enemy->type, enemy->health, enemy->damage, enemy->defense, enemy->speed);
    printf("________________________________________________\n");
}//end roundHeader()

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