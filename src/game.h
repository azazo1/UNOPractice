//
// Created by azazo1 on 2023/9/27.
//

#ifndef UNO_GAME_H
#define UNO_GAME_H

#define MAX_PLAYERS 4

#include <stdio.h>
#include "card.h"
#include "player.h"
#include <stdlib.h>
#include <time.h>

typedef enum {
    BEFORE_START, RUNNING, END
} GameState;

typedef struct { // 储存一局游戏中的信息
    int playerCount;
    int currentPlayerIndex;
    int roundCount;
    int availableCardsCount;
    _Bool turningDirection; // 该变量用于翻转卡, true则正向增大, false则反向减小
    GameState gameState;
    Card cardLib[MAX_CARDS]; // 牌库
    _Bool cardOwned[MAX_CARDS]; // 用于储存牌是否被占有
    Player players[MAX_PLAYERS];
} Game;

Game oneGame;

void initGame();

void initCards();

/**
 * 发牌器:
 * 第一次抽牌, 从未占用牌库中选取牌号, 并标记该牌为已占用.
 * 第二次抽牌时, 以剩余牌数为最大量取随机数 i, 依次遍历找到未占用牌库中序号为 i 的牌, 并标记为已占用.
 * */
void distributeCards();

/**
 * 从未占用牌堆中随机选一张牌, 返回该牌在牌堆中的序号, 请在使用前设置随机种子
 * */
int randomSelectAvailableCard();
/**
 * 从可用牌堆中取牌标记为已占用, 但是并不检查此牌是否原来可用
 * */
void markCard(int cardIndex);

#endif //UNO_GAME_H
