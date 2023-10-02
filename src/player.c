//
// Created by azazo1 on 2023/9/27.
//

#include "player.h"

void putCard(Player *player, int cardIndex) {
    player->ownedCards[player->ownedCount++] = cardIndex;
}

_Bool throwCard(Player *player, int cardIndex) {
    int found;
    for (found = 0; found <= player->ownedCount; ++found) { // 等于号表示搜索到了最后也没找到，用于下文判断
        if (player->ownedCards[found] == cardIndex) {
            break;
        }
    }
    if (found >= player->ownedCount) {
        return FALSE;
    }
    for (int i = found; i < player->ownedCount; i++) { // 所有元素往前移
        player->ownedCards[i] = player->ownedCards[i + 1];
    }
    player->ownedCount--;
    return TRUE;
}