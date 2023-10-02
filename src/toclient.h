//
// Created by azazo1 on 2023/9/29.
//

#ifndef UNO_TOCLIENT_H
#define UNO_TOCLIENT_H
#define BUF_SIZE 1024

#include "player.h"
#include "server.h"
#include "msg.h"
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

/**
 * 获取所有玩家中发来的第一条信息并储存到rst(请确保有1024长度)中, 将数据长度储存到 length 中, 内容会包含一个回车符, 如果出现套接字异常, 则返回错误码 -1
 * 返回值是玩家的序号
 * */
int receiveMsg(char *rst, int *length);

/**
 * 向玩家发送消息，带字符串内容
 * */
void sendMsgWithContent(Player *player, char code, char *content);

/**
 * 向玩家发送消息
 * */
void sendMsg(Player *player, char code);

#endif //UNO_TOCLIENT_H
