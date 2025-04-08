//________________________________________________________________________________________________
// Written by..: Mohamad Al Kary                                                                 |
// Date Written: 04/08/2025                                                                   	 |
// Purpose.....: Arena Champions Structs													     |
//________________________________________________________________________________________________
#pragma once

typedef struct PLAYER{
    char name[100];
    int health;
    int attack;
    int defense;
    int wins;
    int losses;
} PLAYER;

typedef struct ENEMY{
    char type[100];
    int health;
    int attack;
    int defense;
} ENEMY;

typedef struct BATTLE{
    char enemyType;
    int result; // win = 1, loss = 0
    int damageTaken;
    int damageDealth;
    int roundCount;
} BATTLE;