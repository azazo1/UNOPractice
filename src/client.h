//
// Created by azazo1 on 2023/9/29.
//

#ifndef UNO_CLIENT_H
#define UNO_CLIENT_H

#include "player.h"
#include "msg.h"

/**
 * 获取所有玩家中发来的第一条信息并储存到rst中
 * 返回值是玩家的序号
 * */
int receiveMsg(char *rst);

/**
 * 向玩家发送消息，带字符串内容
 * */
void sendMsgWithContent(int playerIndex, int code, char *content);

/**
 * 向玩家发送消息
 * */
void sendMsg(int playerIndex, int code);

#endif //UNO_CLIENT_H
