//
// Created by azazo1 on 2023/9/27.
//
#include "game.h"

void gamingLoop() { // 在接收完客户端后调用
    while (oneGame.gameState == RUNNING) {
        char msg[50];
        int msgLength;
        int playerIndex = receiveMsg(msg, &msgLength); // 获取客户端发来的第一条消息
        if (playerIndex == -1) {
            printf("Socket Exception occurred, game stopped.\n");
            oneGame.gameState = END;
            return;
        }
        Player *player = &oneGame.players[playerIndex];
        const char msgType = parseMsgType(msg);
        if (msgType == MSG_PLACE_CARD) {
            // 检查是否是该玩家的回合
            if (playerIndex != oneGame.currentPlayerIndex) {
                sendMsg(player, MSG_NOT_YOUR_TURN);
                continue;
            }
            // 检查此牌是否可出
            char s_cardIndex[5]; // 可能还会追加一个颜色字符
            int cardIndex;
            int length;
            parseMsgContent(msg, s_cardIndex, &length);
            if (sscanf(s_cardIndex, "%d", &cardIndex) == 1) {
                // 检查牌是否在该玩家的手牌中在则打出该牌
                _Bool inHand = throwCard(player, cardIndex);
                if (!inHand) {
                    sendMsg(player, MSG_CARD_NOT_IN_YOUR_HAND);
                    continue;
                }
                // 检查该牌是否可打出(同色同数等问题)
                if (!checkCanPlace(cardIndex)) {
                    sendMsg(player, MSG_CARD_CANNOT_BE_PLACE);
                    putCard(player, cardIndex); // 放回去手牌
                    continue;
                }
                // 卡牌生效
                if (oneGame.cardLib[cardIndex].signal == CHANGE_COLOR || oneGame.cardLib[cardIndex].signal == PLUS4) {
                    char targetColor;
                    // 检查颜色参数是否合理
                    if (sscanf(s_cardIndex, "%*d%c", &targetColor) != 1 ||
                        (targetColor != R && targetColor != G && targetColor != B && targetColor != Y)) {
                        sendMsg(player, MSG_INVALID_ARGUMENT);
                        putCard(player, cardIndex); // 放回去手牌
                        continue;
                    }
                    takeEffectEx(cardIndex, targetColor); // 特殊牌生效
                } else {
                    takeEffect(cardIndex);
                }
                // 放入弃牌堆
                markCardAsThrown(cardIndex);
                // 广播至所有玩家(消息可能与特殊牌效果重复)
                char content[5];
                strcpy(content, "");
                sprintf(content, "%d%3d", playerIndex, cardIndex);
                char cardStr[4];
                cardToStr(&oneGame.cardLib[cardIndex], cardStr);
                printf("Player %d placed card: %s, and it goes to Player %d\n", playerIndex, cardStr,
                       getNextPlayerIndex());
                broadcastWithContent(MSG_PLAYER_PLACE_CARD, content);
                // 进入下一玩家的回合
                enterNextPlayerRound();
            } else {
                sendMsg(player, MSG_INVALID_ARGUMENT);
            }
            // 检查该玩家是否打光卡牌
            if (!player->ownedCount) {
                // 游戏结束, 对每位玩家手牌计分, 广播分数
                oneGame.gameState = END;
                char rst[BUF_SIZE];
                summonResult(rst);
                printf("Player %d won, other scores are: %s\n", playerIndex, rst);
                broadcastWithContent(MSG_GAME_OVER, rst);
            }
        } else if (msgType == MSG_QUERY_CARDS) {
            // 玩家查询自身手牌
            char content[MAX_CARDS * 3 + 1];
            strcpy(content, "");
            for (int i = 0; i < player->ownedCount; i++) {
                int cardIndex = player->ownedCards[i];
                sprintf(content, "%s%3d", content, cardIndex);
            }
            printf("Player %d queried his hand cards, %s\n", playerIndex, content);
            sendMsgWithContent(player, MSG_QUERY_CARDS_RESULT, content);
        } else if (msgType == MSG_SYNC_GAME_INFO) {
            // 发送玩家数(1), 查询者自身的玩家序号(1), 各个玩家的手牌数(玩家数*3), 当前回合玩家序号(1)，弃牌堆顶层的牌号(3)
            char content[1 + 1 + oneGame.playerCount * 3 + 1 + 3 + 1/*'\0'*/];
            strcpy(content, "");
            sprintf(content, "%d", oneGame.playerCount);
            sprintf(content, "%s%d", content, playerIndex);
            for (int i = 0; i < oneGame.playerCount; i++) {
                sprintf(content, "%s%3d", content, oneGame.players[i].ownedCount);
            }
            sprintf(content, "%s%d", content, oneGame.currentPlayerIndex);
            sprintf(content, "%s%3d", content,
                    oneGame.cardThrownCount > 0 ? oneGame.cardThrown[oneGame.cardThrownCount - 1] : -1/*无牌在弃牌堆*/);
            printf("Player %d synced game info\n", playerIndex);
            sendMsgWithContent(player, MSG_SYNC_GAME_INFO_RESULT, content);
        } else if (msgType == MSG_PASS) {
            // 检查是否是该玩家的回合
            if (playerIndex != oneGame.currentPlayerIndex) {
                sendMsg(player, MSG_NOT_YOUR_TURN);
                continue;
            }
            // 广播玩家过牌
            char content[2];
            strcpy(content, "");
            sprintf(content, "%d", playerIndex);
            printf("Player %d passed his turn, and it goes to Player %d\n", playerIndex, getNextPlayerIndex());
            broadcastWithContent(MSG_PLAYER_PASSED, content);
            enterNextPlayerRound();
        } else if (msgType == MSG_DOUBT) {
            // 检查是否是该玩家的回合, 只有在该玩家的回合, 他才能质疑别人
            if (playerIndex != oneGame.currentPlayerIndex) {
                sendMsg(player, MSG_NOT_YOUR_TURN);
                continue;
            }
            char content[2];
            int length;
            parseMsgContent(msg, content, &length);
            int doubtedPlayerIndex;
            if (sscanf(content, "%d", &doubtedPlayerIndex) != 1) {
                sendMsg(player, MSG_INVALID_ARGUMENT);
                continue;
            }
            Player *doubtedPlayer = &oneGame.players[doubtedPlayerIndex];
            if (doubtedPlayer->ownedCount == 1 && !doubtedPlayer->doubted &&
                !doubtedPlayer->claimedUNO) { // 只有一张牌且没被质疑过且没宣布UNO过
                int cardToPutIndex1 = randomlySelectAvailableCard();
                markCardAsDistributed(cardToPutIndex1);
                int cardToPutIndex2 = randomlySelectAvailableCard();
                markCardAsDistributed(cardToPutIndex2);
                putCard(doubtedPlayer, cardToPutIndex1);
                putCard(doubtedPlayer, cardToPutIndex2);
                // 是否质疑成功(len:1), 质疑者(len:1), 被质疑者(len:1), 若质疑成功则追加两张牌的牌号(len:2*3)
                char response[1 + 1 + 1 + 3 + 3];
                strcpy(response, "");
                sprintf(response, "%d", 1);
                sprintf(response, "%s%d", response, playerIndex);
                sprintf(response, "%s%d", response, doubtedPlayerIndex);
                sprintf(response, "%s%3d", response, cardToPutIndex1);
                sprintf(response, "%s%3d", response, cardToPutIndex2);
                printf("Player %d doubted Player %d and succeed, put cards:%3d %3d\n", playerIndex, doubtedPlayerIndex,
                       cardToPutIndex1, cardToPutIndex2);
                broadcastWithContent(MSG_PLAYER_DOUBTED, response);
                doubtedPlayer->doubted = TRUE;
            } else {
                char response[1 + 1 + 1];
                strcpy(response, "");
                sprintf(response, "%d", 0);
                sprintf(response, "%s%d", response, playerIndex);
                sprintf(response, "%s%d", response, doubtedPlayerIndex);
                printf("Player %d doubted Player %d but failed\n", playerIndex, doubtedPlayerIndex);
                broadcastWithContent(MSG_PLAYER_DOUBTED, response);
            }
        } else if (msgType == MSG_UNO) {
            // 检查是否是该玩家的回合
            if (playerIndex != oneGame.currentPlayerIndex) {
                sendMsg(player, MSG_NOT_YOUR_TURN);
                continue;
            }
            // 玩家宣布UNO
            if (player->ownedCount == 1) {
                char content[2];
                sprintf(content, "%d", playerIndex);
                printf("Player %d claimed UNO\n", playerIndex);
                broadcastWithContent(MSG_PLAYER_UNOED, content);
            } else {
                sendMsg(player, MSG_UNO_FAILED);
            }
        }
    }
}

void takeEffectEx(int cardIndex, char targetColor) {
    // 改变当前颜色和牌类型
    oneGame.currentColor = targetColor;
    oneGame.currentSignal = CHAR_MAX;
    Card *card = &oneGame.cardLib[cardIndex];
    Player *player = &oneGame.players[getNextPlayerIndex()];
    if (card->signal == PLUS4) { // +4牌
        char content[1 + 12 + 1]; // 玩家序号(1) 牌号(4*3) \0(1)
        sprintf(content, "%d", player->index);
        // 加牌
        for (int _ = 0; _ < 4; _++) {
            int cardToPut = randomlySelectAvailableCard();
            putCard(player, cardToPut);
            sprintf(content, "%3d", cardToPut);
        }
        // 玩家继续出牌
        oneGame.currentPlayerIndex = getPrePlayerIndex();
        // 广播
        broadcastWithContent(MSG_PLUS4, content);
    } else { // 变色牌
        // 广播
        char color[2] = {oneGame.currentColor, '\0'};
        broadcastWithContent(MSG_CHANGE_COLOR, color);
    }
}

void takeEffect(int cardIndex) {
    Card *card = &oneGame.cardLib[cardIndex];
    switch (card->signal) {
        case BAN: {
            char s_playerIndex[2];
            sprintf(s_playerIndex, "%d", getNextPlayerIndex());
            oneGame.currentPlayerIndex = getNextPlayerIndex(); // 直接跳过该角色, 不清除被质疑标记和UNO标记
            broadcastWithContent(MSG_BAN_PLAYER, s_playerIndex);
            break;
        }
        case REVERT: {
            oneGame.turningDirection = !oneGame.turningDirection;
            char s_turningDirection[2];
            sprintf(s_turningDirection, "%d", oneGame.turningDirection);
            broadcastWithContent(MSG_REVERT, s_turningDirection);
            break;
        }
        case PLUS2: {
            char content[1 + 3 + 3 + 1];
            sprintf(content, "%d", getNextPlayerIndex());
            oneGame.currentPlayerIndex = getNextPlayerIndex(); // 直接跳过该角色, 不清除被质疑标记和UNO标记
            for (int _ = 0; _ < 2; _++) {
                int cardToPut = randomlySelectAvailableCard();
                putCard(&oneGame.players[oneGame.currentPlayerIndex], cardToPut);
                markCardAsDistributed(cardToPut);
                sprintf(content, "%3d", cardToPut);
            }
            broadcastWithContent(MSG_PLUS2, content);
            break;
        }
        default: // 数字牌
            break;
    }
    // 变色 变标签
    oneGame.currentColor = card->color;
    oneGame.currentSignal = card->signal;
}


void initGame() {
    oneGame.gameState = BEFORE_START;
    oneGame.currentColor = CHAR_MAX;
    oneGame.currentSignal = CHAR_MAX;
    oneGame.playerCount = 0;
    oneGame.currentPlayerIndex = 0;
    oneGame.turningDirection = TRUE;
    oneGame.availableCardsCount = MAX_CARDS;
    oneGame.cardThrownCount = 0;
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

void initPlayers() {
    for (int i = 0; i < oneGame.playerCount; ++i) {
        oneGame.players[i].client = &sockets[i + 1];
        oneGame.players[i].index = i;
        oneGame.players[i].ownedCount = 0;
        oneGame.players[i].doubted = FALSE;
    }
}

void distributeCards() {
    srand(time(NULL));
    for (int playerIndex = 0; playerIndex < oneGame.playerCount; ++playerIndex) {
        for (int _ = 0; _ < 7; _++) { // 每个角色取七张牌
            int target = randomlySelectAvailableCard();
            // 选到目标牌
            markCardAsDistributed(target);
            putCard(&oneGame.players[playerIndex], target);
        }
    }
}

int randomlySelectAvailableCard() {
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

void markCardAsDistributed(int cardIndex) {
    oneGame.cardOwned[cardIndex] = TRUE;
    oneGame.availableCardsCount--;
}

void markCardAsThrown(int cardIndex) {
    oneGame.cardThrown[oneGame.cardThrownCount] = cardIndex;
    oneGame.cardThrownCount++;
}

void broadcast(char code) {
    for (int i = 0; i < oneGame.playerCount; ++i) {
        sendMsg(&oneGame.players[i], code);
    }
}

void broadcastWithContent(char code, char *content) {
    for (int i = 0; i < oneGame.playerCount; ++i) {
        sendMsgWithContent(&oneGame.players[i], code, content);
    }
}

_Bool checkCanPlace(int cardIndex) {
    if (oneGame.currentColor == CHAR_MAX) { // 全局第一张牌不用判断
        return TRUE;
    }
    Card *card = &oneGame.cardLib[cardIndex];
    return card->signal == oneGame.currentSignal || card->color == oneGame.currentColor || card->color == NO_COLOR;
}

int getNextPlayerIndex() {
    return (oneGame.currentPlayerIndex + (oneGame.turningDirection ? 1 : -1)) % oneGame.playerCount;
}

int getPrePlayerIndex() {
    return (oneGame.currentPlayerIndex - (oneGame.turningDirection ? 1 : -1)) % oneGame.playerCount;
}

void summonResult(char *content) {
    char rst[BUF_SIZE];
    for (int i = 0; i < oneGame.playerCount; ++i) {
        int score = 0;
        for (int j = 0; j < oneGame.players[j].ownedCount; ++j) {
            int cardIndex = oneGame.players[j].ownedCards[j];
            Card *card = &oneGame.cardLib[cardIndex];
            if (0 <= card->signal && card->signal <= 9) { // 数字牌计5分
                score += 5;
            } else if (10 <= card->signal && card->signal <= 12) { // 功能牌计20分
                score += 20;
            } else if (13 <= card->signal && card->signal <= 14) { // 万能拍计40分
                score += 40;
            }
        }
        sprintf(rst, "%5d", score);
    }
    strcpy(content, rst);
}

void randomlyChooseStarter() {
    oneGame.currentPlayerIndex = rand() % oneGame.playerCount;
}

void enterNextPlayerRound() {
    // 清除自身被质疑过标记和UNO标记
    oneGame.currentPlayerIndex = getNextPlayerIndex();
    oneGame.players[oneGame.currentPlayerIndex].doubted = FALSE;
    oneGame.players[oneGame.currentPlayerIndex].claimedUNO = FALSE;
}