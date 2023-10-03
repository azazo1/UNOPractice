//
// Created by azazo1 on 2023/9/29.
//

#ifndef UNO_TOCLIENT_H
#define UNO_TOCLIENT_H
#define BUF_SIZE 1024

#include "player.h"
#include "server.h"
#include "msg.h"
#include <stdarg.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
char bufs[MAX_SOCKET][BUF_SIZE]; // 缓冲区 行, 列 (一行一行存储)
/**
 * 初始化缓冲区
 * */
void initBuf();

/**
 * 获取所有玩家中发来的第一条信息并储存到rst(请确保有1024长度)中, 将数据长度储存到 length 中, 内容不包含回车符, 如果出现套接字异常, 则返回错误码 -1
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

/**
 * 拼接两个字符串, 但 src 和 dest 地址可以一样
 * */
int sprintfSafely(char *dest, const char *format, ...);

#endif //UNO_TOCLIENT_H
