//
// Created by azazo1 on 2023/9/27.
//
#include "game.h"

void initGame() {
    oneGame.gameState = BEFORE_START;
    oneGame.playerCount = 0;
    oneGame.currentPlayerIndex = 0;
    oneGame.roundCount = 0;
    oneGame.turningDirection = TRUE;
    oneGame.availableCardsCount = MAX_CARDS;
    for (int i = 0; i < MAX_CARDS; ++i) {
        oneGame.cardOwned[i] = FALSE;
    }
    initCards();
}

void initCards() {
    int ptr = 0;
    for (char color = 0; color < 4; color++) { // NUMBER_0 各色只有一张
        Card card;
        card.signal = 0;
        card.color = color;
        oneGame.cardLib[ptr++] = card;
    }

    for (char signal = 1; signal < 13; signal++) { // 13 和 14 号不用添加颜色单独讨论
        for (char color = 0; color < 4; ++color) { // 四种颜色
            for (char _ = 0; _ < 2; _++) { // 各色两张
                Card card;
                card.signal = signal;
                card.color = color;
                oneGame.cardLib[ptr++] = card;
            }
        }
    }

    for (char signal = 13; signal < 15; signal++) {
        for (char _ = 0; _ < 4; _++) {
            Card card;
            card.signal = signal;
            card.color = NO_COLOR;
            oneGame.cardLib[ptr++] = card;
        }
    }
}

void distributeCards() {
    srand(time(NULL));
    for (int playerIndex = 0; playerIndex < oneGame.playerCount; ++playerIndex) {
        for (int _ = 0; _ < 7; _++) { // 每个角色取七张牌
            int target = randomSelectAvailableCard();
            // 选到目标牌
            markCard(target);
            putCard(&oneGame.players[playerIndex], target);
        }
    }
}

int randomSelectAvailableCard() {
    int selected = rand() % oneGame.availableCardsCount; // 第 selected 个可用的牌
    int skipped = 0; // 已跳过的可用牌数
    int target = 0; // 目标牌索引
    while (skipped < selected || oneGame.cardOwned[target]) {
        if (!oneGame.cardOwned[target]) {
            skipped++;
        }
        target++;
    }
    return target;
}

void markCard(int cardIndex) {
    oneGame.cardOwned[cardIndex] = TRUE;
    oneGame.availableCardsCount--;
}