//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 04/08/2025                                                                   	 |
// Purpose.....: Arena Champions Structs													     |
//________________________________________________________________________________________________
#pragma once

typedef struct PLAYER{
    char name[101];
    int maxHealth;
    int health;
    int damage;
    int baseDefense;
    int defense;    //add a perry mechanic where %75 of enemy damage is given to enemy at a 33% chance
    int isDefending; //1 = true | 0 = false
    int speed; //how many turns the player gets in a row
    int gold;
    int level;
    int wins;
    int losses;
} PLAYER;

typedef struct ENEMY{
    char type[100];
    int ID;
    int health;
    int baseDamage;
    int damage;
    int defense;
    int speed;  //how many turns the enemy gets in a row
} ENEMY;

typedef struct BATTLE{
    char enemyType[101];
    char result[10]; // lose = 1, win = 2, tie = 3
    int damageTaken;
    int damageDealt;
    int roundCount;
    int ID;
} BATTLE;