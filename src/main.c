//
// Created by azazo1 on 2023/9/27.
//

#include "game.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please provide a player number\n");
        return 1;
    }
    int playerNumber;
    if (sscanf(argv[1], "%d", &playerNumber) != 1) {
        printf("Invalid player number\n");
        return 1;
    }
    printf("Target player number is %d\n", playerNumber);
    initGame();
    if (preGaming(playerNumber)) {
        return 1;
    }
    printf("Players ready\n");
    oneGame.playerCount = playerNumber;
    oneGame.gameState = RUNNING;
    printf("Initializing players...\n");
    initBuf(); // 初始化套接字接收数据缓冲区
    initPlayers();
    printf("Distributing cards...\n");
    distributeCards();
    randomlyChooseStarter();
    printf("Game start\n");
    gamingLoop();
    printf("Game over\n");
    closeAllSocket();
    return 0;
}
