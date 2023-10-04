//
// Created by azazo1 on 2023/9/27.
//

#ifndef UNO_GAME_H
#define UNO_GAME_H

#define MAX_PLAYERS 4

#include <stdio.h>
#include "card.h"
#include "player.h"
#include "toclient.h"
#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef enum {
    BEFORE_START, RUNNING, END
} GameState;

typedef struct { // 储存一局游戏中的信息
    int playerCount;
    int currentPlayerIndex;
    int currentColor; // 当前可打出的颜色, 初始为 char 最大值
    char currentSignal; // 当前可打出的牌类型, 初始为 char 最大值
    int availableCardsCount;
    _Bool turningDirection; // 该变量用于翻转卡, true则正向增大, false则反向减小
    GameState gameState;
    Card cardLib[MAX_CARDS]; // 牌库
    _Bool cardOwned[MAX_CARDS]; // 用于储存牌是否被占有
    int cardThrown[MAX_CARDS]; // 弃牌库
    int cardThrownCount; // 已弃牌数
    Player players[MAX_PLAYERS];
} Game;

Game oneGame;

void initGame();

void initCards();

/**
 * 初始化玩家结构体
 * */
void initPlayers();

/**
 * 发牌器:
 * 第一次抽牌, 从未占用牌库中选取牌号, 并标记该牌为已占用.
 * 第二次抽牌时, 以剩余牌数为最大量取随机数 i, 依次遍历找到未占用牌库中序号为 i 的牌, 并标记为已占用.
 * */
void distributeCards();

/**
 * 从未占用牌堆中随机选一张牌, 返回该牌在牌堆中的序号, 请在使用前设置随机种子
 * */
int randomlySelectAvailableCard();

/**
 * 从可用牌堆中取牌标记为已占用, 但是并不检查此牌是否原来可用
 * */
void markCardAsDistributed(int cardIndex);

/**
 * 游戏主循环
 * */
void gamingLoop();

/**
 * 广播消息至所有玩家
 * */
void broadcast(char code);

/**
 * 广播消息至所有玩家，带内容
 * */
void broadcastWithContent(char code, char *content);

/**
 * 将牌放入弃牌堆
 * */
void markCardAsThrown(int cardIndex);

/**
 * 卡牌生效, 只对除了 变色 和 +4 以外的牌能正常起作用
 * */
void takeEffect(int cardIndex);

/**
 * 特殊卡牌(+4/变色)生效
 * */
void takeEffectEx(int cardIndex, int targetColor);

/**
 * 检查该牌是否可被发出
 * */
_Bool checkCanPlace(int cardIndex);

/**
 * 获取下一位出牌玩家的序号
 * */
int getNextPlayerIndex();

/**
 * 获取上一位出牌玩家的序号
 * */
int getPrePlayerIndex();

/**
 * 产生游戏得分结果
 * */
void summonResult(char *rst);

/**
 * 随机选取第一个出牌的玩家
 * */
void randomlyChooseStarter();

/**
 * 进入下一个玩家的回合
 * */
void enterNextPlayerRound();

/**
 * 检查玩家手牌上是否有牌可以打出
 * */
_Bool checkHaveCardToPlace(Player *player);

#endif //UNO_GAME_H
