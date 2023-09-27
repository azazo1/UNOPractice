//
// Created by azazo1 on 2023/9/27.
//
#include "game.h"


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

int sendCards() {

}