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
} Player;

/**
 * 向该玩家手牌上放牌
 * */
void putCard(Player *player, int cardIndex);

/**
 * 玩家从手牌上出牌, 返回是否成功出牌
 * */
_Bool throwCard(Player *player, int cardIndex);

#endif //UNO_PLAYER_H
