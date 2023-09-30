//
// Created by azazo1 on 2023/9/27.
//
#include "game.h"

void gamingLoop() { // 在接收完客户端后调用
    while (oneGame.gameState == RUNNING) { // todo 添加结束判断
        char msg[50];
        int playerIndex = receiveMsg(msg); // 获取客户端发来的第一条消息
        const char msgType = parseMsgType(msg);
        if (msgType == MSG_PLACE_CARD) {
            // 检查是否是该玩家的回合
            if (playerIndex != oneGame.currentPlayerIndex) {
                sendMsg(playerIndex, MSG_NOT_YOUR_TURN);
                continue;
            }
            // 检查此牌是否可出
            char s_cardIndex[5]; // 可能还会追加一个颜色字符
            int cardIndex;
            parseMsgContent(msg, s_cardIndex);
            if (sscanf(s_cardIndex, "%d", &cardIndex) == 1) {
                // 检查牌是否在该玩家的手牌中在则打出该牌
                _Bool inHand = throwCard(&oneGame.players[playerIndex], cardIndex);
                if (!inHand) {
                    sendMsg(playerIndex, MSG_CARD_NOT_IN_YOUR_HAND);
                    continue;
                }
                // 检查该牌是否可打出(同色同数等问题)
                if (!checkCanPlace(cardIndex)) {
                    sendMsg(playerIndex, MSG_CARD_CANNOT_BE_PLACE);
                    putCard(&oneGame.players[playerIndex], cardIndex); // 放回去手牌
                    continue;
                }
                // 卡牌生效
                if (oneGame.cardLib[cardIndex].signal == CHANGE_COLOR || oneGame.cardLib[cardIndex].signal == PLUS4) {
                    char targetColor;
                    // 检查颜色参数是否合理
                    if (sscanf(s_cardIndex, "%*d%c", &targetColor) != 1 ||
                        (targetColor != R && targetColor != G && targetColor != B && targetColor != Y)) {
                        sendMsg(playerIndex, MSG_INVALID_ARGUMENT);
                        putCard(&oneGame.players[playerIndex], cardIndex); // 放回去手牌
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
                sprintf(content, "%d%3d", playerIndex, cardIndex);
                broadcastWithContent(MSG_PLAYER_PLACE_CARD, content);
                // 进入下一玩家的回合 todo 质疑UNO的部分建议直接给每个玩家多个质疑键用于质疑任意一个玩家
                oneGame.currentPlayerIndex = getNextPlayerIndex();
            } else {
                sendMsg(playerIndex, MSG_INVALID_ARGUMENT);
            }
        } else if (msgType == MSG_QUERY_CARDS) {
            // 玩家查询自身手牌
            char content[MAX_CARDS * 3 + 1];
            for (int i = 0; i < oneGame.players[playerIndex].ownedCount; i++) {
                int cardIndex = oneGame.players[playerIndex].ownedCards[i];
                sprintf(content, "%3d", cardIndex);
            }
            sendMsgWithContent(playerIndex, MSG_QUERY_CARDS_RESULT, content);
        } else if (msgType == MSG_SYNC_GAME_INFO) {
            // 发送玩家数(1)，各个玩家的手牌数(玩家数*3)，当前回合玩家序号(1)，弃牌堆顶层的牌号(3), \0(1)
            char content[1 + oneGame.playerCount * 3 + 1 + 3 + 1];
            sprintf(content, "%d", oneGame.playerCount);
            for (int i = 0; i < oneGame.playerCount; i++) {
                sprintf(content, "%3d", oneGame.players[i].ownedCount);
            }
            sprintf(content, "%d", oneGame.currentPlayerIndex);
            sprintf(content, "%d", oneGame.cardThrown[oneGame.cardThrownCount - 1]);
            sendMsgWithContent(playerIndex, MSG_SYNC_GAME_INFO_RESULT, content);
        }
    }
}

void takeEffectEx(int cardIndex, char targetColor) {
    // todo 变色 广播 加牌 封禁 +4牌回滚一个玩家以便玩家继续发牌 将currentSignal变为 CHAR_MAX
    oneGame.currentColor = targetColor;
    oneGame.currentSignal = CHAR_MAX;
    Card *card = &oneGame.cardLib[cardIndex];
    if (card->signal == PLUS4) { // +4牌

    } else { // 变色牌

    }
}

void takeEffect(int cardIndex) {
    Card *card = &oneGame.cardLib[cardIndex];
    switch (card->signal) {
        case BAN: {
            char s_playerIndex[2];
            sprintf(s_playerIndex, "%d", getNextPlayerIndex());
            oneGame.currentPlayerIndex = getNextPlayerIndex(); // 直接跳过该角色
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
            oneGame.currentPlayerIndex = getNextPlayerIndex(); // 直接跳过该角色
            for (int _ = 0; _ < 2; _++) {
                int cardToPut = randomSelectAvailableCard();
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

void distributeCards() {
    srand(time(NULL));
    for (int playerIndex = 0; playerIndex < oneGame.playerCount; ++playerIndex) {
        for (int _ = 0; _ < 7; _++) { // 每个角色取七张牌
            int target = randomSelectAvailableCard();
            // 选到目标牌
            markCardAsDistributed(target);
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

void markCardAsDistributed(int cardIndex) {
    oneGame.cardOwned[cardIndex] = TRUE;
    oneGame.availableCardsCount--;
}

void markCardAsThrown(int cardIndex) {
    oneGame.cardThrown[oneGame.cardThrownCount] = cardIndex;
    oneGame.cardThrownCount++;
}

void broadcast(int code) {
    for (int i = 0; i < oneGame.playerCount; ++i) {
        sendMsg(i, code);
    }
}

void broadcastWithContent(int code, char *content) {
    for (int i = 0; i < oneGame.playerCount; ++i) {
        sendMsgWithContent(i, code, content);
    }
}

_Bool checkCanPlace(int cardIndex) { // todo 添加初始时对 CHAR_MAX 的判断
    Card *card = &oneGame.cardLib[cardIndex];
    return card->signal == oneGame.currentSignal || card->color == oneGame.currentColor || card->color == NO_COLOR;
}

inline int getNextPlayerIndex() {
    return oneGame.currentPlayerIndex + (oneGame.turningDirection ? 1 : -1);
}