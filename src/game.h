//
// Created by azazo1 on 2023/9/27.
//

#ifndef UNO_GAME_H
#define UNO_GAME_H

#include <stdio.h>
#include "card.h"

typedef enum {
    BEFORE_START, RUNNING, END
} GameState;

typedef struct { // 储存一局游戏中的信息
    int playerCount;
    int currentPlayerIndex;
    int roundCount;
    _Bool tuningDirection; // 该变量用于翻转卡, true则正向增大, false则反向减小
    GameState gameState;
    Card cardLib[MAX_CARDS]; // 牌库
    _Bool cardOwned[MAX_CARDS]; // 用于储存牌是否被占有
} Game;

Game oneGame;

void initCards();

int sendCards();

#endif //UNO_GAME_H
