//
// Created by azazo1 on 2023/9/27.
//

#ifndef UNO_PLAYER_H
#define UNO_PLAYER_H

#include "card.h"

typedef struct {
    int index;
    int ownedCards[MAX_CARDS];
    int ownedCount; // 拥有牌数
    // todo 实现套接字操作 Client *client;
} Player;

/**
 * 向该玩家手牌上放牌
 * */
void putCard(Player *player, int cardIndex);

/**
 * 玩家从手牌上出牌, 返回该牌原本是否在手牌中
 * */
_Bool throwCard(Player *player, int cardIndex);

#endif //UNO_PLAYER_H
