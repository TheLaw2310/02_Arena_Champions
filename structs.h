//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 04/08/2025                                                                   	 |
// Purpose.....: Arena Champions Structs													     |
//________________________________________________________________________________________________
#pragma once

typedef struct PLAYER{
    char name[101];
    int health;
    int damage;
    int defense;
    int wins;
    int losses;
} PLAYER;

typedef struct ENEMY{
    char type[100];
    int health;
    int damage;
    int defense;
} ENEMY;

typedef struct BATTLE{
    char enemyType[101];
    int result; // lose = 1, win = 2, tie = 3
    int damageTaken;
    int damageDealt;
    int roundCount;
    int ID;
} BATTLE;