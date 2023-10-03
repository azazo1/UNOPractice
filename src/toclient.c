//
// Created by azazo1 on 2023/9/29.
//

#include "toclient.h"

/**
 * 把字符串转换为数字数组的可见格式
 * */
void debugStr(char *dst, char *src) {
    _Bool first = TRUE;
    strcpy(dst, "[");
    int length = strlen(src);
    for (int i = 0; i < length; i++) {
        if (first) {
            sprintfSafely(dst, "%s%d", dst, src[i]);
            first = FALSE;
        } else {
            sprintfSafely(dst, "%s, %d", dst, src[i]);
        }
    }
    sprintfSafely(dst, "%s]", dst);
}

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
    for (int i = 1; i < n_socket; i++) {
        int playerIndex = i - 1;
        // 先检查缓冲区内有没有完整消息
        char *returnChar;
        if (strlen(bufs[i]) && (returnChar = strchr(bufs[i], '\n')) != NULL) { // bufs 中已经有完整的消息了
            *length = (int) (returnChar - bufs[i]) / (int) sizeof(char);
            strncpy(rst, bufs[i], *length);
            rst[*length] = '\0';
            // 去除 bufs[i] 内的第一条消息
            char temp[BUF_SIZE];
            strcpy(temp, (bufs[i] + *length + 1)); // 从回车符后一个字符开始复制, 由于 bufs 是全局变量, 初始化后内容全是 '\0', 不需要担心无效字符问题
            strcpy(bufs[i], temp);
            return playerIndex;
        }
    }

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
                    int playerIndex = i - 1;

                    // 到此处时, bufs[i] 内只有残缺的消息片段或为空
                    char buf0[BUF_SIZE];
                    strcpy(buf0, "");
                    int getLength = recv(sockets[i], buf0, BUF_SIZE, 0);
                    buf0[getLength] = '\0'; // recv 函数不会在后面补 \0 !!!

                    if (getLength <= 0) {
                        // 套接字异常, 返回错误码
                        return -1;
                    }

                    getLength = sprintfSafely(buf0, "%s%s", bufs[i], buf0); // 把残缺消息片段补在前面

                    char *returnChar;
                    while ((returnChar = strchr(buf0, '\n')) == NULL) { // 直到读取到'\n'才结束
                        char buf[BUF_SIZE];
                        strcpy(buf, "");
                        int newLength = recv(sockets[i], buf, BUF_SIZE, 0); // 多读一段
                        if (newLength > 0) { // 正常情况
                            strcat(buf0, buf);
                            getLength += newLength;
                        } else {
                            // 套接字异常, 返回错误码
                            return -1;
                        }
                    }

                    char temp[BUF_SIZE];
                    *length = (int) (returnChar - buf0) / (int) sizeof(char);
                    *returnChar = '\0';// 替换回车符为 \0 以便复制第一条消息到
                    strcpy(rst, buf0);

                    getLength -= *length + 1;

                    // 去除 buf0 内的第一条消息, 并将剩余内容复制到 bufs[i] 内
                    // 注意！！！这里 buf0在原回车符后的内容由于初始化时没有清零，值是随机的，不一定是从套接字接受得到的，需要一个长度参数判断后面的字符是否为有效字符
                    strncpy(temp, (buf0 + *length + 1), getLength); // 从回车符(被替换为\0)后一个字符开始复制
                    temp[getLength] = '\0';
                    strcpy(bufs[i], temp);
                    return playerIndex; // 返回玩家序号
                }
            }
        }
        Sleep(20);
    }
}

void initBuf() {
    for (int i = 0; i < MAX_SOCKET; ++i) {
        strcpy(bufs[i], "");
    }
}

int sprintfSafely(char *dest, const char *format, ...) {
    char temp[BUF_SIZE];
    va_list args;
    int i;
    va_start(args, format);
    i = vsprintf(temp, format, args);
    va_end(args);
    strcpy(dest, temp);
    return i;
}