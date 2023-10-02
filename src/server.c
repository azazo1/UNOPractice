//
// Created by azazo1 on 2023/10/1.
//

#include "server.h"

int preGaming(const int n_client) {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }
    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Failed to create server socket\n");
        WSACleanup();
        return 1;
    }
    // 绑定端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to bind server on port %d\n", SERVER_PORT);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // 监听
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Failed to let server listen\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    addSocket(&serverSocket, &serverAddr, sizeof(serverAddr));
    printf("Server listened on port: %d\n", SERVER_PORT);
    // 接受客户端
    while (n_socket - 1 < n_client) { // 去掉服务端套接字再对比
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (clientSocket == SOCKET_ERROR) {
            continue;
        }
        addSocket(&clientSocket, &clientAddr, clientAddrLen);
        printf("New client: %s,%d, need %d more\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),
               n_client - (n_socket - 1));
    }
    return 0;
}

void addSocket(const SOCKET *socket, const struct sockaddr_in *addr, const int addrLen) {
    sockets[n_socket] = *socket;
    socketAddresses[n_socket] = *addr;
    socketAddressLengths[n_socket] = addrLen;
    n_socket++;
}

void closeAllSocket() {
    for (int i = 0; i < n_socket; ++i) {
        closesocket(sockets[i]);
    }
    WSACleanup();
}