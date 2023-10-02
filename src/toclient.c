//
// Created by azazo1 on 2023/9/29.
//

#include "toclient.h"

void sendMsg(Player *player, char code) {
    char buf[2] = {code, '\n'};
    int sent = send(*player->client, buf, 2, 0);
    if (sent < 0) {
        return;
    }
    while (sent < 2) {
        int increment = send(*player->client, &buf[sent], 2 - sent, 0);
        if (increment < 0) {
            return;
        }
    }
}

void sendMsgWithContent(Player *player, char code, char *content) {
    int len = (int) strlen(content) + 1/*消息代号*/ + 1/*回车符*/;
    char buf[len];
    strcpy(buf, "");
    sprintf(buf, "%c%s\n", code, content);
    int sent = send(*player->client, buf, len, 0); // 末尾的 ‘\0' 由于长度限制不会被发送
    while (sent < len) {
        sent += send(*player->client, &buf[sent], len - sent, 0);
    }
}

int receiveMsg(char *rst, int *length) {
    fd_set readFDs;
    FD_ZERO(&readFDs);
    for (int i = 1; i < n_socket; ++i) {
        FD_SET(sockets[i], &readFDs);
    }
    while (1) {
        int ret = select(0/* Windows下该参数可忽略*/, &readFDs, NULL, NULL, NULL);
        if (ret > 0) {
            for (int i = 1; i < n_socket; ++i) {
                if (FD_ISSET(sockets[i], &readFDs)) {
                    _Bool noError = TRUE;
                    char buf0[BUF_SIZE];
                    strcpy(buf0, "");
                    int getLength = recv(sockets[i], buf0, BUF_SIZE, 0);
                    while (strchr(buf0, '\n') == NULL) { // 直到读取到'\n'才结束 todo 有吞掉下一行的风险
                        char buf[BUF_SIZE];
                        strcpy(buf, "");
                        int newLength = recv(sockets[i], buf, BUF_SIZE, 0); // 多读一段
                        if (newLength > 0) { // 正常情况
                            getLength += newLength;
                            strcat(buf0, buf);
                        } else {
                            // 套接字异常, 返回错误码
                            noError = FALSE;
                            return -1;
                        }
                    }
                    if (noError) {
                        *length = getLength;
                        strcpy(rst, buf0);
                        return i - 1; // 返回玩家序号
                    }
                }
            }
        }
        Sleep(20);
    }
}