//
// Created by azazo1 on 2023/9/27.
//

#include "game.h"

int main() {
    initCards();
    for (int i = 0; i < MAX_CARDS; i++) {
        char out[4];
        cardToStr(&oneGame.cardLib[i], out);
        printf("%s\n", out);
    }
    return 0;
}
