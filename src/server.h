//
// Created by azazo1 on 2023/10/1.
//

#ifndef UNO_SERVER_H
#define UNO_SERVER_H

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#define  SERVER_PORT 12345
SOCKET sockets[10];
struct sockaddr_in socketAddresses[10];
int socketAddressLengths[10];
int n_socket; // 包括服务端在内的套接字总数
/**
 * 开始游戏前创建服务端套接字和接受客户端套接字, 当客户端套接字达到一定数量后完成
 * 返回是否出现异常
 * */
int preGaming(int n_client);

void addSocket(const SOCKET *socket, const struct sockaddr_in *addr, int addrLen);

void closeAllSocket();

#endif //UNO_SERVER_H
